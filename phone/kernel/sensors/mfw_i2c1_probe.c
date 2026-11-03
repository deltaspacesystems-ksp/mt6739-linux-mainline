// SPDX-License-Identifier: GPL-2.0-only
/* DURA-L21 bounded PIO diagnostic adapter. LTR577 reads only, no discovery,
 * clock/power/pin changes, IRQ, DMA or sensor configuration writes.
 * Based on the measured MT6739 charger combined-read transaction.
 */
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/ioport.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/platform_device.h>
#include <linux/platform_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/delay.h>

static bool enable;
static bool ir_test;
module_param(ir_test,bool,0400);
MODULE_PARM_DESC(ir_test,"Explicit bounded5-second stock IR trial");
static int ir_result=-ENODATA;
static bool ir_restored;
static unsigned ir_samples;
static u16 ir_values[50];
static bool als_control;
module_param(als_control, bool, 0400);
MODULE_PARM_DESC(als_control, "Permit only MAIN_CTRL0/2 for bounded ALS-only trial");
module_param(enable, bool, 0400);
MODULE_PARM_DESC(enable, "Explicitly permit LTR577-only PIO diagnostic adapter");
struct probe_bus {
 struct i2c_adapter adapter;
 struct platform_device *parent;
 void __iomem *regs, *infra;
 struct resource *claim;
 bool faulted;
};
static struct probe_bus *bus;
static bool allowed(const struct i2c_msg *m, int n)
{
 unsigned int r, len;
 /* Exact stock MC3xxx identity registers only; no probing other addresses. */
 if(n==2 && m[0].addr==0x4c && m[1].addr==0x4c &&
    !(m[0].flags & ~I2C_M_DMA_SAFE) &&
    (m[1].flags & ~I2C_M_DMA_SAFE)==I2C_M_RD &&
    m[0].len==1 && m[1].len==1 && m[0].buf && m[1].buf &&
    (m[0].buf[0]==0x3b || m[0].buf[0]==0x18))return true;
 if (als_control && n == 1 && m[0].addr == 0x53 &&
     !(m[0].flags & ~I2C_M_DMA_SAFE) && m[0].len == 2 && m[0].buf &&
     m[0].buf[0] == 0 && (m[0].buf[1] == 0 || m[0].buf[1] == 2))
  return true;
 if (n != 2 || m[0].addr != 0x53 || m[1].addr != 0x53 ||
     (m[0].flags & ~I2C_M_DMA_SAFE) ||
     (m[1].flags & ~I2C_M_DMA_SAFE) != I2C_M_RD || m[0].len != 1 ||
     !m[0].buf || !m[1].buf)
  return false;
 r = m[0].buf[0]; len = m[1].len;
 return (r <= 6 && len == 1) ||
        (r == 8 && len == 2) || ((r == 10 || r == 13) && len == 3);
}
static int transfer_impl(struct i2c_adapter *adapter, struct i2c_msg *m, int n, bool internal)
{
 struct probe_bus *b = i2c_get_adapdata(adapter);
 const unsigned int offsets[] = {4, 8, 0x10, 0x14, 0x18, 0x6c};
 u16 saved[ARRAY_SIZE(offsets)], status;
 unsigned int i, count;
 bool writing = n == 1;
 int ret;
 if (!allowed(m, n) && !(internal && n==1 && m[0].addr==0x53 && !m[0].flags && m[0].len==2 && m[0].buf && m[0].buf[0]<=3)) return -EOPNOTSUPP;
 /* A latched bus permits one bounded best-effort PS-off command only. */
 if (b->faulted && !(internal && n==1 && m[0].buf[0]==0 && m[0].buf[1]==0)) return -EIO;
 if (readl(b->infra + 0x90) & BIT(12)) return -EHOSTDOWN;
 if ((readw(b->regs + 0x24) & 1) || ((readw(b->regs + 0x30) >> 4) & 15))
  return -EBUSY;
 for (i = 0; i < ARRAY_SIZE(offsets); i++) saved[i] = readw(b->regs + offsets[i]);
 writew(saved[1] & ~7, b->regs + 8);
 writew(0x1ff, b->regs + 0xc);
 writew(1, b->regs + 0x38);
 writew(writing ? 0x28 : 0x3a, b->regs + 0x10);
 writew(m[0].addr << 1, b->regs + 4);
 writew(writing ? 2 : 1, b->regs + 0x14);
 writew(writing ? 0 : m[1].len, b->regs + 0x6c);
 writew(writing ? 1 : 2, b->regs + 0x18);
 writew(m[0].buf[0], b->regs);
 if (writing) writew(m[0].buf[1], b->regs);
 writew(1, b->regs + 0x24);
 ret = readw_poll_timeout(b->regs + 0xc, status, status & 7, 50, 20000);
 if (!ret && (status & 7) != 1) ret = -EREMOTEIO;
 count = (readw(b->regs + 0x30) >> 4) & 15;
 if (!ret && count != (writing ? 0 : m[1].len)) ret = -EIO;
 if (!ret && !writing)
  for (i = 0; i < m[1].len; i++) m[1].buf[i] = readw(b->regs) & 255;
 writew(status, b->regs + 0xc);
 /* A failed transaction latches the adapter and pins module ownership.
  * Never reset clocks/controller or retry blindly after an unknown fault. */
 if (ret) {
  if(!b->faulted)__module_get(THIS_MODULE);
  b->faulted = true;
  pr_err("mfw-i2c1-probe: transaction failed %d; adapter pinned until reboot\n", ret);
 }
 if (!(readw(b->regs + 0x24) & 1)) {
  writew(1, b->regs + 0x38);
  for (i = 0; i < ARRAY_SIZE(offsets); i++) writew(saved[i], b->regs + offsets[i]);
 } else if (!b->faulted) {
  b->faulted = true;
  __module_get(THIS_MODULE);
  ret = -EBUSY;
 }
 return ret ? ret : n;
}
static int transfer(struct i2c_adapter *adapter,struct i2c_msg *m,int n)
{
 return transfer_impl(adapter,m,n,false);
}
static int ir_transfer_read(u8 reg, u8 *data, u16 len)
{
 struct i2c_msg m[]={{.addr=0x53,.len=1,.buf=&reg},{.addr=0x53,.flags=I2C_M_RD,.len=len,.buf=data}};
 int rc=transfer_impl(&bus->adapter,m,2,true);
 return rc==2?0:rc<0?rc:-EIO;
}
static int ir_transfer_write(u8 reg,u8 value)
{
 u8 data[]={reg,value};struct i2c_msg m={.addr=0x53,.len=2,.buf=data};
 int rc=transfer_impl(&bus->adapter,&m,1,true);
 return rc==1?0:rc<0?rc:-EIO;
}
static ssize_t ir_trial_show(struct device *dev,struct device_attribute *attr,char *buf)
{
 ssize_t len;unsigned i;
 i2c_lock_bus(&bus->adapter,I2C_LOCK_ROOT_ADAPTER);
 len=sysfs_emit(buf,"result=%d restored=%u samples=%u\n",ir_result,ir_restored,ir_samples);
 for(i=0;i<ir_samples;i++)len+=sysfs_emit_at(buf,len,"%u %u\n",i,ir_values[i]);
 i2c_unlock_bus(&bus->adapter,I2C_LOCK_ROOT_ADAPTER);
 return len;
}
static ssize_t ir_trial_store(struct device *dev,struct device_attribute *attr,const char *buf,size_t count)
{
 u8 original[4],id,data[2],check;unsigned i,saved=0;int rc=0,restore=0;
 if(!ir_test||!sysfs_streq(buf,"run"))return -EPERM;
 i2c_lock_bus(&bus->adapter,I2C_LOCK_ROOT_ADAPTER);
 ir_samples=0;ir_restored=false;ir_result=-EINPROGRESS;
 rc=ir_transfer_read(6,&id,1);
 if(rc||id!=0xc2){if(!rc)rc=-ENODEV;goto done;}
 for(i=0;i<4;i++){rc=ir_transfer_read(i,&original[i],1);if(rc)goto done;saved++;}
 if(original[0]){rc=-EBUSY;goto done;}
 /* Exact stock4.4 PS defaults, including the non-b1 path. No IRQ/NVRAM. */
 rc=ir_transfer_write(1,0x36);if(rc)goto cleanup;
 rc=ir_transfer_write(2,0x10);if(rc)goto cleanup;
 rc=ir_transfer_write(3,0x5c);if(rc)goto cleanup;
 rc=ir_transfer_write(0,1);if(rc)goto cleanup;
 for(i=0;i<50;i++){
  msleep(100);
  rc=ir_transfer_read(8,data,2);if(rc)break;
  ir_values[ir_samples++]=data[0]|((data[1]&7)<<8);
 }
cleanup:
 /* This callback completes restoration even if the writing process exits. */
 if(ir_transfer_write(0,0))restore=-EIO;
 for(i=3;i>0;i--)if(ir_transfer_write(i,original[i]))restore=-EIO;
 for(i=0;i<4;i++){
  int rr=ir_transfer_read(i,&check,1);
  if(rr||check!=original[i])restore=-EIO;
 }
 ir_restored=!restore;
 if(restore)rc=restore;
done:
 ir_result=rc;
 if(saved<4)ir_restored=false;
 i2c_unlock_bus(&bus->adapter,I2C_LOCK_ROOT_ADAPTER);
 if(rc)return rc;
 return count;
}
static DEVICE_ATTR_RW(ir_trial);

static u32 functionality(struct i2c_adapter *a)
{
 return I2C_FUNC_I2C;
}
static const struct i2c_algorithm algorithm = {
 .master_xfer = transfer, .functionality = functionality,
};
static int __init probe_init(void)
{
 struct device_node *node;
 struct resource resource;
 int ret;
 if (!enable) return -EPERM;
 node = of_find_node_by_path("/i2c@11008000");
 if (!node) return -ENODEV;
 ret = of_address_to_resource(node, 0, &resource);
 if (ret || resource.start != 0x11008000 || resource_size(&resource) < 0x70) {
  of_node_put(node); return -EINVAL;
 }
 bus = kzalloc(sizeof(*bus), GFP_KERNEL);
 if (!bus) { of_node_put(node); return -ENOMEM; }
 bus->parent = of_find_device_by_node(node);
 of_node_put(node);
 if (!bus->parent) { ret = -ENODEV; goto free; }
 device_lock(&bus->parent->dev);
 if (device_is_bound(&bus->parent->dev)) { ret = -EBUSY; goto unlock; }
 bus->claim = request_mem_region(resource.start, resource_size(&resource), "mfw-i2c1-probe");
 if (!bus->claim) { ret = -EBUSY; goto unlock; }
 bus->regs = ioremap(resource.start, resource_size(&resource));
 bus->infra = ioremap(0x10001000, 0x1000);
 if (!bus->regs || !bus->infra) { ret = -ENOMEM; goto unmap; }
 if (readl(bus->infra + 0x90) & BIT(12)) { ret = -EHOSTDOWN; goto unmap; }
 bus->adapter.owner = THIS_MODULE;
 bus->adapter.algo = &algorithm;
 bus->adapter.dev.parent = &bus->parent->dev;
 bus->adapter.nr = 1;
 bus->adapter.timeout = msecs_to_jiffies(25);
 strscpy(bus->adapter.name, "MICROFW LTR577-only PIO diagnostic");
 i2c_set_adapdata(&bus->adapter, bus);
 ret = i2c_add_numbered_adapter(&bus->adapter);
 if (ret) goto unmap;
 ret=device_create_file(&bus->adapter.dev,&dev_attr_ir_trial);
 if(ret){i2c_del_adapter(&bus->adapter);goto unmap;}
 device_unlock(&bus->parent->dev);
 pr_info("mfw-i2c1-probe: adapter1 ready; LTR577 allowlisted reads only\n");
 return 0;
unmap:
 if (bus->infra) iounmap(bus->infra);
 if (bus->regs) iounmap(bus->regs);
 release_mem_region(resource.start, resource_size(&resource));
unlock:
 device_unlock(&bus->parent->dev);
 put_device(&bus->parent->dev);
free:
 kfree(bus); bus = NULL;
 return ret;
}
static void __exit probe_exit(void)
{
 device_remove_file(&bus->adapter.dev,&dev_attr_ir_trial);
 i2c_del_adapter(&bus->adapter);
 iounmap(bus->infra); iounmap(bus->regs);
 release_mem_region(bus->claim->start, resource_size(bus->claim));
 put_device(&bus->parent->dev);
 kfree(bus);
}
module_init(probe_init);
module_exit(probe_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MICROFW bounded LTR577-only diagnostic PIO adapter");
