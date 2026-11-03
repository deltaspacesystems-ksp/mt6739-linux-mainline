// SPDX-License-Identifier: GPL-2.0-only
/* MT6739 transitional RDMA IRQ observer, NOT a KMS or GPU driver.
 * Default: observe/ack RDMA. Optional bounded DMA test writes OVL3 address
 * on EOF, then restores the original. Never frees an allocated test buffer.
 * Exclusive resource/IRQ ownership. Never run /dev/mem probes concurrently.
 * Future KMS must replace this owner, not bind alongside it.
 */
#include <linux/atomic.h>
#include <linux/completion.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include "mfw-frame-switch.h"
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/sysfs.h>

#ifdef MODULE
#error "Bringup observer must be built-in; LK keeps the interrupt enabled"
#endif

#define RDMA_INT_ENABLE 0x00
#define RDMA_INT_STATUS 0x04
#define RDMA_GLOBAL_CON 0x10
#define RDMA_FRAME_START BIT(1)
#define RDMA_FRAME_END BIT(2)
#define RDMA_ABNORMAL BIT(3)
#define RDMA_UNDERFLOW BIT(4)
#define RDMA_KNOWN_INTERRUPTS GENMASK(5, 0)

struct mfw_rdma_observer {
 void __iomem *regs;
 atomic64_t irqs, eof, sof, underflow, abnormal;
 atomic_t last_status;
 struct device *dev;
 void __iomem *ovl, *larb, *dsi;
 void *boot_image, *cpu[2];
 dma_addr_t dma[2];
 spinlock_t flip_lock;
 struct mutex test_lock;
 struct completion flip_done;
 struct mfw_frame_switch flip;
 atomic_t test_state;
 bool test_enabled;
};

#define MFW_FRAME_BYTES (720U * 1440U * 4U)
#define MFW_ORIGINAL_ADDR 0x775eb000U
#define MFW_OVL3_ADDR 0xfa0
/* Retain DMA allocations for the built-in driver's entire life. In particular,
 * no timeout/error path frees memory which might still be scanned out. */
static void mfw_flip_eof(struct mfw_rdma_observer *o)
{
 unsigned long flags;
 spin_lock_irqsave(&o->flip_lock, flags);
 switch (mfw_switch_eof(&o->flip)) {
 case MFW_SWITCH_APPLY:
  writel(o->flip.target, o->ovl + MFW_OVL3_ADDR);
  readl(o->ovl + MFW_OVL3_ADDR); /* Drain posted address write. */
  break;
 case MFW_SWITCH_COMPLETE:
  complete(&o->flip_done);
  break;
 default: break;
 }
 spin_unlock_irqrestore(&o->flip_lock, flags);
}

static irqreturn_t mfw_rdma_irq(int irq, void *data)
{
 struct mfw_rdma_observer *o = data;
 u32 status = readl(o->regs + RDMA_INT_STATUS);
 u32 enabled = readl(o->regs + RDMA_INT_ENABLE);

 if (!(status & enabled & RDMA_KNOWN_INTERRUPTS))
  return IRQ_NONE;
 /* Same acknowledge convention as upstream mtk_disp_rdma.c. */
 writel(0, o->regs + RDMA_INT_STATUS);
 atomic_set(&o->last_status, status);
 atomic64_inc(&o->irqs);
 if (status & RDMA_FRAME_END) {
  atomic64_inc(&o->eof);
  if (o->test_enabled)
   mfw_flip_eof(o);
 }
 if (status & RDMA_FRAME_START)
  atomic64_inc(&o->sof);
 if (status & RDMA_UNDERFLOW)
  atomic64_inc(&o->underflow);
 if (status & RDMA_ABNORMAL)
  atomic64_inc(&o->abnormal);
 return IRQ_HANDLED;
}

static ssize_t counters_show(struct device *dev,
 struct device_attribute *attr, char *buf)
{
 struct mfw_rdma_observer *o = dev_get_drvdata(dev);

 /* Independent monotonic counters; not an atomic multi-counter snapshot. */
 return sysfs_emit(buf,
  "irqs=%lld eof=%lld sof=%lld underflow=%lld abnormal=%lld last_status=0x%x\n",
  (long long)atomic64_read(&o->irqs), (long long)atomic64_read(&o->eof),
  (long long)atomic64_read(&o->sof), (long long)atomic64_read(&o->underflow),
  (long long)atomic64_read(&o->abnormal), atomic_read(&o->last_status));
}
static bool mfw_test_guard(struct mfw_rdma_observer *o)
{
 return readl(o->ovl + 0x2c) == 8 &&
  readl(o->ovl + 0x90) == 0x21ff &&
  readl(o->ovl + 0x98) == 0x05a002d0 &&
  (readl(o->ovl + 0xa4) & 0xffff) == 2880 &&
  !(readl(o->larb + 0x380) & 1) &&
  (readl(o->dsi + 0x14) & 3) == 1;
}
static int mfw_test_switch(struct mfw_rdma_observer *o, u32 addr)
{
 unsigned long flags;
 int ret;
 if (!mfw_test_guard(o))
  return -ENODEV;
 reinit_completion(&o->flip_done);
 spin_lock_irqsave(&o->flip_lock, flags);
 ret = mfw_switch_request(&o->flip, addr);
 spin_unlock_irqrestore(&o->flip_lock, flags);
 if (ret)
  return ret;
 if (!wait_for_completion_timeout(&o->flip_done, msecs_to_jiffies(500)))
  return -ETIMEDOUT;
 return readl(o->ovl + MFW_OVL3_ADDR) == addr ? 0 : -EIO;
}
static void mfw_test_restore_fallback(struct mfw_rdma_observer *o)
{
 unsigned long flags;
 spin_lock_irqsave(&o->flip_lock, flags);
 mfw_switch_cancel(&o->flip);
 /* Recovery only, when no reliable EOF remains: may visibly tear once. */
 writel(MFW_ORIGINAL_ADDR, o->ovl + MFW_OVL3_ADDR);
 readl(o->ovl + MFW_OVL3_ADDR);
 spin_unlock_irqrestore(&o->flip_lock, flags);
}
static ssize_t flip_test_store(struct device *dev, struct device_attribute *attr,
 const char *buf, size_t count)
{
 struct mfw_rdma_observer *o = dev_get_drvdata(dev);
 unsigned i, x, y;
 int ret = 0, restore;
 s64 underflow, abnormal;
 if (!o->test_enabled || !sysfs_streq(buf, "run"))
  return -EINVAL;
 if (!mutex_trylock(&o->test_lock))
  return -EBUSY;
 if (!mfw_test_guard(o) || readl(o->ovl + MFW_OVL3_ADDR) != MFW_ORIGINAL_ADDR) {
  ret = -ENODEV;
  goto out;
 }
 atomic_set(&o->test_state, 1);
 underflow = atomic64_read(&o->underflow);
 abnormal = atomic64_read(&o->abnormal);
 for (i = 0; i < 2; i++) {
  u64 end;
  if (!o->cpu[i])
   o->cpu[i] = dma_alloc_coherent(dev, PAGE_ALIGN(MFW_FRAME_BYTES),
    &o->dma[i], GFP_KERNEL);
  if (!o->cpu[i]) { ret = -ENOMEM; goto out; }
  end = (u64)o->dma[i] + MFW_FRAME_BYTES - 1;
  if (!o->dma[i] || (o->dma[i] & 4095) || end > U32_MAX) {
   ret = -ERANGE; goto out;
  }
  dev_info(dev, "DMA test buffer%u addr=%pad bytes=%u\n", i, &o->dma[i], MFW_FRAME_BYTES);
  memcpy(o->cpu[i], o->boot_image, MFW_FRAME_BYTES);
  for (y = 128; y < 224; y++)
   for (x = 64; x < 320; x++)
    ((u32 *)o->cpu[i])[y * 720 + x] = i ? 0xffff00ff : 0xff00ff50;
 }
 dma_wmb();
 dev_info(dev, "DMA flip test: green5s, magenta5s, original; no DSI kick\n");
 for (i = 0; i < 2; i++) {
  ret = mfw_test_switch(o, (u32)o->dma[i]);
  if (ret)
   break;
  msleep(5000);
 }
 restore = mfw_test_switch(o, MFW_ORIGINAL_ADDR);
 if (restore) {
  mfw_test_restore_fallback(o);
  if (!ret) ret = restore;
 }
 if (!ret && (atomic64_read(&o->underflow) != underflow ||
              atomic64_read(&o->abnormal) != abnormal))
  ret = -EIO;
 dev_info(dev, "DMA flip test result=%d restore=%d original_readback=%08x\n",
  ret, restore, readl(o->ovl + MFW_OVL3_ADDR));
out:
 atomic_set(&o->test_state, ret ? ret : 2);
 mutex_unlock(&o->test_lock);
 return ret ? ret : count;
}
static ssize_t flip_state_show(struct device *dev, struct device_attribute *attr, char *buf)
{
 struct mfw_rdma_observer *o = dev_get_drvdata(dev);
 /* 0 idle,1 running,2 register/counter checks passed,negative errno.
  * State2 still requires visual confirmation of both buffers on the panel. */
 return sysfs_emit(buf, "%d\n", atomic_read(&o->test_state));
}
static DEVICE_ATTR_RO(counters);
static DEVICE_ATTR_WO(flip_test);
static DEVICE_ATTR_RO(flip_state);
static struct attribute *mfw_rdma_attrs[] = {
 &dev_attr_counters.attr, &dev_attr_flip_test.attr, &dev_attr_flip_state.attr, NULL
};
ATTRIBUTE_GROUPS(mfw_rdma);

static int mfw_rdma_probe(struct platform_device *pdev)
{
 struct device *dev = &pdev->dev;
 struct mfw_rdma_observer *o;
 struct resource *res;
 int irq, ret;

 /* Explicit DT opt-in; never attach to stock mediatek,disp_rdma0. */
 if (!device_property_read_bool(dev, "microfw,observe-lk-scanout"))
  return -ENODEV;
 res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
 if (!res || res->start != 0x1400b000 || resource_size(res) != 0x1000)
  return dev_err_probe(dev, -EINVAL, "Unexpected RDMA resource\n");
 o = devm_kzalloc(dev, sizeof(*o), GFP_KERNEL);
 if (!o)
  return -ENOMEM;
 o->regs = devm_ioremap_resource(dev, res);
 if (IS_ERR(o->regs))
  return PTR_ERR(o->regs);
 /* Existing stream and EOF interrupt must already be enabled by LK.
  * Do not repair/reset/modeset anything here. */
 if (!(readl(o->regs + RDMA_GLOBAL_CON) & BIT(0)) ||
     (readl(o->regs + RDMA_GLOBAL_CON) & BIT(1)) ||
     !(readl(o->regs + RDMA_INT_ENABLE) & RDMA_FRAME_END))
  return dev_err_probe(dev, -ENODEV, "Expected running LK direct-link with EOF enabled\n");
 irq = platform_get_irq(pdev, 0);
 if (irq < 0)
  return irq;
 o->dev = dev;
 spin_lock_init(&o->flip_lock);
 mutex_init(&o->test_lock);
 init_completion(&o->flip_done);
 if (device_property_read_bool(dev, "microfw,scanout-dma-test")) {
  /* Exact resources are intentional bringup guards, not a generic binding. */
  const resource_size_t expected[] = { 0x1400a000, 0x14003000, 0x14012000 };
  void __iomem **mapping[] = { &o->ovl, &o->larb, &o->dsi };
  unsigned i;
  for (i = 0; i < 3; i++) {
   res = platform_get_resource(pdev, IORESOURCE_MEM, i + 1);
   if (!res || res->start != expected[i] || resource_size(res) != 4096)
    return -EINVAL;
   if (i == 0)
    *mapping[i] = devm_ioremap_resource(dev, res);
   else
    *mapping[i] = devm_ioremap(dev, res->start, resource_size(res));
   if (IS_ERR_OR_NULL(*mapping[i]))
    return -ENOMEM;
  }
  if (!mfw_test_guard(o) || readl(o->ovl + MFW_OVL3_ADDR) != MFW_ORIGINAL_ADDR)
   return dev_err_probe(dev, -ENODEV, "DMA test guard mismatch\n");
  ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32));
  if (ret) return ret;
  o->boot_image = devm_memremap(dev, MFW_ORIGINAL_ADDR, MFW_FRAME_BYTES, MEMREMAP_WC);
  if (IS_ERR(o->boot_image)) return PTR_ERR(o->boot_image);
  o->test_enabled = true;
 }
 platform_set_drvdata(pdev, o);
 ret = devm_request_irq(dev, irq, mfw_rdma_irq, 0, dev_name(dev), o);
 if (ret)
  return dev_err_probe(dev, ret, "RDMA IRQ request failed\n");
 dev_info(dev, "LK scanout observer IRQ=%d; observer; optional DMA test only after explicit sysfs request\n", irq);
 return 0;
}

static const struct of_device_id mfw_rdma_match[] = {
 { .compatible = "microfw,mt6739-rdma-observer" }, {}
};
MODULE_DEVICE_TABLE(of, mfw_rdma_match);
static struct platform_driver mfw_rdma_driver = {
 .probe = mfw_rdma_probe,
 .driver = {
  .name = "mfw-rdma-observer",
  .of_match_table = mfw_rdma_match,
  .dev_groups = mfw_rdma_groups,
  /* Do not offer unbind while the firmware-owned interrupt stays enabled.
   * Observer is built-in for bringup, replaces no power management. */
  .suppress_bind_attrs = true,
 },
};
module_platform_driver(mfw_rdma_driver);
MODULE_DESCRIPTION("MICROFW MT6739 LK RDMA frame IRQ observer (not KMS)");
MODULE_LICENSE("GPL");
