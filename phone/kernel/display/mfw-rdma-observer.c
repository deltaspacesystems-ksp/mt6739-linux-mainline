// SPDX-License-Identifier: GPL-2.0-only
/* MT6739 transitional RDMA IRQ observer, NOT a KMS or GPU driver.
 * Keeps LK scanout intact. Sole MMIO write: acknowledge RDMA INT_STATUS.
 * Exclusive resource/IRQ ownership. Never run /dev/mem probes concurrently.
 * Future KMS must replace this owner, not bind alongside it.
 */
#include <linux/atomic.h>
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
};

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
 if (status & RDMA_FRAME_END)
  atomic64_inc(&o->eof);
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
static DEVICE_ATTR_RO(counters);
static struct attribute *mfw_rdma_attrs[] = { &dev_attr_counters.attr, NULL };
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
 platform_set_drvdata(pdev, o);
 ret = devm_request_irq(dev, irq, mfw_rdma_irq, 0, dev_name(dev), o);
 if (ret)
  return dev_err_probe(dev, ret, "RDMA IRQ request failed\n");
 dev_info(dev, "LK scanout observer IRQ=%d; no modeset/address/clock writes\n", irq);
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
