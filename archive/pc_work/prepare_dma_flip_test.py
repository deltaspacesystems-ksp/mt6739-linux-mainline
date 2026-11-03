from pathlib import Path
w=Path(r'D:\y5_linux_work');d=w/'microfw-display'
(d/'mfw-frame-switch.h').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_FRAME_SWITCH_H
#define MFW_FRAME_SWITCH_H
/* Caller serializes request/EOF/cancel and owns/pins every buffer. No MMIO.
 * Two additional EOFs after APPLY are a conservative bringup hold, NOT proof
 * of hardware latch semantics or a final KMS presentation timestamp. */
#ifdef __KERNEL__
#include <linux/errno.h>
#include <linux/types.h>
#else
#include <errno.h>
#include <stdint.h>
typedef uint32_t u32;
#endif
struct mfw_frame_switch { u32 target; unsigned pending, settling; };
enum mfw_switch_event { MFW_SWITCH_NONE, MFW_SWITCH_APPLY, MFW_SWITCH_COMPLETE };
static inline int mfw_switch_request(struct mfw_frame_switch *s,u32 target){
 if(s->pending||s->settling)return -EBUSY;
 if(!target||(target&4095))return -EINVAL;
 s->target=target;s->pending=1;return 0;
}
static inline enum mfw_switch_event mfw_switch_eof(struct mfw_frame_switch *s){
 if(s->pending){s->pending=0;s->settling=2;return MFW_SWITCH_APPLY;}
 if(s->settling&&!--s->settling)return MFW_SWITCH_COMPLETE;
 return MFW_SWITCH_NONE;
}
static inline void mfw_switch_cancel(struct mfw_frame_switch *s){*s=(struct mfw_frame_switch){0};}
#endif
''',newline='\n')
(d/'test_frame_switch.c').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include "mfw-frame-switch.h"
int main(void){struct mfw_frame_switch s={0};
 assert(mfw_switch_request(&s,0)==-EINVAL);assert(mfw_switch_request(&s,1)==-EINVAL);
 for(unsigned i=0;i<100;i++){
  assert(!mfw_switch_request(&s,0x60000000));assert(mfw_switch_request(&s,0x61000000)==-EBUSY);
  assert(mfw_switch_eof(&s)==MFW_SWITCH_APPLY&&s.target==0x60000000);
  assert(mfw_switch_request(&s,0x61000000)==-EBUSY);
  assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);assert(mfw_switch_eof(&s)==MFW_SWITCH_COMPLETE);
  assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);
 }
 assert(!mfw_switch_request(&s,0x60000000));mfw_switch_cancel(&s);assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);
 assert(!mfw_switch_request(&s,0x60000000));assert(mfw_switch_eof(&s)==MFW_SWITCH_APPLY);
 mfw_switch_cancel(&s);assert(mfw_switch_eof(&s)==MFW_SWITCH_NONE);assert(!mfw_switch_request(&s,0x61000000));
 puts("PASS: serialized frame switch, conservative retirement delay, idle EOF, cancellation and invalid targets");return 0;}
''',newline='\n')
s=(d/'mfw-rdma-observer.c').read_text()
s=s.replace('#include <linux/atomic.h>','#include <linux/atomic.h>\n#include <linux/completion.h>\n#include <linux/dma-mapping.h>\n#include <linux/delay.h>\n#include <linux/mutex.h>\n#include <linux/spinlock.h>\n#include "mfw-frame-switch.h"')
s=s.replace('atomic_t last_status;','''atomic_t last_status;
 struct device *dev;
 void __iomem *ovl, *larb, *dsi;
 void *boot_image, *cpu[2];
 dma_addr_t dma[2];
 spinlock_t flip_lock;
 struct mutex test_lock;
 struct completion flip_done;
 struct mfw_frame_switch flip;
 atomic_t test_state;
 bool test_enabled;''')
s=s.replace('static irqreturn_t mfw_rdma_irq','''#define MFW_FRAME_BYTES (720U * 1440U * 4U)
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

static irqreturn_t mfw_rdma_irq''',1)
s=s.replace('if (status & RDMA_FRAME_END)\n  atomic64_inc(&o->eof);','''if (status & RDMA_FRAME_END) {
  atomic64_inc(&o->eof);
  if (o->test_enabled)
   mfw_flip_eof(o);
 }''')
a=s.index('static DEVICE_ATTR_RO(counters);')
b=s.index('ATTRIBUTE_GROUPS(mfw_rdma);',a)+len('ATTRIBUTE_GROUPS(mfw_rdma);')
s=s[:a]+'''static bool mfw_test_guard(struct mfw_rdma_observer *o)
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
  memcpy(o->cpu[i], o->boot_image, MFW_FRAME_BYTES);
  for (y = 128; y < 224; y++)
   for (x = 64; x < 320; x++)
    ((u32 *)o->cpu[i])[y * 720 + x] = i ? 0xffff00ff : 0xff00ff50;
 }
 dma_wmb();
 dev_info(dev, "DMA flip test: green5s, magenta5s, original; no DSI kick\\n");
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
 dev_info(dev, "DMA flip test result=%d restore=%d original_readback=%08x\\n",
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
 return sysfs_emit(buf, "%d\\n", atomic_read(&o->test_state));
}
static DEVICE_ATTR_RO(counters);
static DEVICE_ATTR_WO(flip_test);
static DEVICE_ATTR_RO(flip_state);
static struct attribute *mfw_rdma_attrs[] = {
 &dev_attr_counters.attr, &dev_attr_flip_test.attr, &dev_attr_flip_state.attr, NULL
};
ATTRIBUTE_GROUPS(mfw_rdma);'''+s[b:]
s=s.replace('platform_set_drvdata(pdev, o);','''o->dev = dev;
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
   return dev_err_probe(dev, -ENODEV, "DMA test guard mismatch\\n");
  ret = dma_set_mask_and_coherent(dev, DMA_BIT_MASK(32));
  if (ret) return ret;
  o->boot_image = devm_memremap(dev, MFW_ORIGINAL_ADDR, MFW_FRAME_BYTES, MEMREMAP_WC);
  if (IS_ERR(o->boot_image)) return PTR_ERR(o->boot_image);
  o->test_enabled = true;
 }
 platform_set_drvdata(pdev, o);''',1)
s=s.replace('no modeset/address/clock writes\\n','observer; optional DMA test only after explicit sysfs request\\n')
s=s.replace(' * Keeps LK scanout intact. Sole MMIO write: acknowledge RDMA INT_STATUS.', ' * Default: observe/ack RDMA. Optional bounded DMA test writes OVL3 address\n * on EOF, then restores the original. Never frees an allocated test buffer.')
(d/'mfw-rdma-dma-test.c').write_text(s,newline='\n')
(w/'compile_dma_flip_test.sh').write_text('''#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cp /mnt/d/y5_linux_work/microfw-display/mfw-rdma-dma-test.c drivers/gpu/drm/mfw-bringup/mfw-rdma-dma-test.c
cp /mnt/d/y5_linux_work/microfw-display/mfw-frame-switch.h drivers/gpu/drm/mfw-bringup/
printf 'obj-y += mfw-rdma-dma-test.o\\n' > drivers/gpu/drm/mfw-bringup/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/gpu/drm/mfw-bringup/mfw-rdma-dma-test.o
cd /mnt/d/y5_linux_work/microfw-display
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_frame_switch.c -o /tmp/mfw-test-frame-switch
/tmp/mfw-test-frame-switch
''',newline='\n')
print('Prepared bounded DMA buffer switch at EOF; no automatic address changes on boot')
