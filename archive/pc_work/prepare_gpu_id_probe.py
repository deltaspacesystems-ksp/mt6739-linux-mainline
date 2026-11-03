from pathlib import Path
w=Path(r'D:\y5_linux_work');d=w/'microfw-gpu'
(d/'mfw-gpu-id.h').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_GPU_ID_H
#define MFW_GPU_ID_H
#ifdef __KERNEL__
#include <linux/errno.h>
#include <linux/types.h>
#else
#include <errno.h>
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif
struct mfw_gpu_id { unsigned b,v,n,c; };
/* Linux7.3pvr_gpuid_decode_reg layout, no apphint or compiled-BVNC fallback. */
static inline int mfw_gpu_decode_id(u32 core_id,u64 pbvnc,struct mfw_gpu_id *id){
 id->b=(pbvnc>>48)&65535;
 if(id->b){id->v=(pbvnc>>32)&65535;id->n=(pbvnc>>16)&65535;id->c=pbvnc&65535;}
 else {id->b=(pbvnc>>16)&255;id->v=(pbvnc>>8)&255;id->n=(core_id>>8)&255;id->c=core_id&255;}
 return !id->b||id->b==65535||!id->n?-ENODEV:0;
}
#endif
''',newline='\n')
(d/'test_gpu_id.c').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include "mfw-gpu-id.h"
int main(void){struct mfw_gpu_id id;
 assert(!mfw_gpu_decode_id(0,22ULL<<48|49ULL<<32|21ULL<<16|16,&id));assert(id.b==22&&id.v==49&&id.n==21&&id.c==16);
 assert(!mfw_gpu_decode_id(21<<8|16,22<<16|49<<8,&id));assert(id.b==22&&id.v==49&&id.n==21&&id.c==16);
 assert(mfw_gpu_decode_id(0,0,&id)==-ENODEV);assert(mfw_gpu_decode_id(~0U,~0ULL,&id)==-ENODEV);
 puts("PASS: PBVNC/legacy register decode, zero/all-ones rejection; synthetic cases not hardware evidence");return 0;}
''',newline='\n')
(d/'mfw-gpu-id-probe.c').write_text('''// SPDX-License-Identifier: GPL-2.0-only
/* Bounded26MHz MFG power/ID bringup probe. NOT a rendering driver.
 * No firmware, IRQ, jobs, GPU writes, HWAPM, voltage or PLL changes.
 * No GPU mapping/read until both powered-domain acknowledgements succeed.
 * Probe operates only on explicit sysfs request, not automatically on boot.
 */
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include "mfw-mfg-power-sequence.h"
#include "mfw-gpu-id.h"
#ifdef MODULE
#error "Bringup power/ID probe must remain built-in"
#endif
struct mfw_gpu_probe {
 struct device *dev;
 struct clk *clock;
 void __iomem *blocks[3];
 struct mutex lock;
 struct mfw_gpu_id id;
 u64 raw_bvnc;
 u32 raw_core_id;
 int last_result, voltage_uv;
 bool faulted;
};
static u32 mfw_probe_read(void *ctx,enum mfw_mfg_block block,u32 off)
{
 return readl(((struct mfw_gpu_probe *)ctx)->blocks[block]+off);
}
static void mfw_probe_write(void *ctx,enum mfw_mfg_block block,u32 off,u32 value)
{
 writel(value,((struct mfw_gpu_probe *)ctx)->blocks[block]+off);
}
static int mfw_probe_poll(void *ctx,enum mfw_mfg_block block,u32 off,u32 mask,u32 wanted)
{
 u32 value;
 return readl_poll_timeout(((struct mfw_gpu_probe *)ctx)->blocks[block]+off,
  value,(value&mask)==wanted,10,100000);
}
static int mfw_probe_idle(void *ctx)
{
 u32 value;
 return readl_poll_timeout(((struct mfw_gpu_probe *)ctx)->blocks[MFW_MFG_INFRA],
  value,(value&BIT(24))||!(value&GENMASK(15,14)),10,100000);
}
static bool mfw_probe_domains_off(struct mfw_gpu_probe *p)
{
 return !((mfw_probe_read(p,0,0x180)|mfw_probe_read(p,0,0x184))&0x80000010);
}
static ssize_t identity_show(struct device *dev,struct device_attribute *attr,char *buf)
{
 struct mfw_gpu_probe *p=dev_get_drvdata(dev);
 ssize_t size;
 mutex_lock(&p->lock);
 size=sysfs_emit(buf,"result=%d faulted=%u voltage_uv=%d clock_hz=%lu bvnc=%u.%u.%u.%u core_id=%08x pbvnc=%016llx\\n",
  p->last_result,p->faulted,p->voltage_uv,clk_get_rate(p->clock),p->id.b,p->id.v,p->id.n,p->id.c,
  p->raw_core_id,(unsigned long long)p->raw_bvnc);
 mutex_unlock(&p->lock);
 return size;
}
static ssize_t probe_id_store(struct device *dev,struct device_attribute *attr,const char *buf,size_t count)
{
 struct mfw_gpu_probe *p=dev_get_drvdata(dev);
 struct device_node *np;
 struct platform_device *pmic;
 struct regulator *vcore;
 struct mfw_mfg_io io={.ctx=p,.read=mfw_probe_read,.write=mfw_probe_write,.poll=mfw_probe_poll,.idle=mfw_probe_idle};
 void __iomem *gpu;
 bool clock_held=false;
 int ret,down;
 if(!sysfs_streq(buf,"read-id"))return -EINVAL;
 if(!mutex_trylock(&p->lock))return -EBUSY;
 if(p->faulted){ret=-EIO;goto out;}
 np=of_parse_phandle(dev->of_node,"microfw,pmic-provider",0);
 if(!np){ret=-ENODEV;goto out;}
 pmic=of_find_device_by_node(np);of_node_put(np);
 if(!pmic){ret=-EPROBE_DEFER;goto out;}
 /* Prevent the existing touch wrapper's short PMIC unbind during the test.
  * No permanent regulator consumer ref: release before PMIC device unlock. */
 device_lock(&pmic->dev);
 if(!pmic->dev.driver){ret=-ENODEV;goto unlock_pmic;}
 vcore=regulator_get(dev,"vcore");
 if(IS_ERR(vcore)){ret=PTR_ERR(vcore);goto unlock_pmic;}
 p->voltage_uv=regulator_get_voltage(vcore);
 ret=p->voltage_uv;
 if(ret<0)goto put_vcore;
 if(ret<1050000||ret>1312500){ret=-ERANGE;goto put_vcore;}
 ret=regulator_is_enabled(vcore);
 if(ret<=0){if(!ret)ret=-EHOSTDOWN;goto put_vcore;}
 if(!mfw_probe_domains_off(p)||
    (mfw_probe_read(p,0,0x338)&127)!=0x72||
    (mfw_probe_read(p,0,0x33c)&127)!=0x72){ret=-EBUSY;goto put_vcore;}
 ret=clk_set_rate(p->clock,26000000);
 if(ret)goto put_vcore;
 ret=clk_prepare_enable(p->clock);
 if(ret)goto put_vcore;
 clock_held=true;
 if(clk_get_rate(p->clock)!=26000000){ret=-ERANGE;goto release_clock;}
 dev_info(dev,"Explicit GPU ID probe at26MHz, VCORE=%duV; no firmware\\n",p->voltage_uv);
 ret=mfw_mfg_power_up(&io);
 if(ret){p->faulted=true;dev_err(dev,"Power-up stopped at stage%u: %d; retaining clock\\n",io.stage,ret);goto release_clock;}
 if((mfw_probe_read(p,0,0x338)&127)!=13||
    (mfw_probe_read(p,0,0x33c)&127)!=13){ret=-EIO;goto power_down;}
 /* Deliberately map only after power/SRAM/reset/clock checks, never in probe. */
 gpu=ioremap(0x13000000,4096);
 if(!gpu){ret=-ENOMEM;goto power_down;}
 p->raw_core_id=readl(gpu+0x18);
 p->raw_bvnc=readq(gpu+0x20);
 iounmap(gpu);
 ret=mfw_gpu_decode_id(p->raw_core_id,p->raw_bvnc,&p->id);
 dev_info(dev,"GPU raw CORE_ID=%08x PBVNC=%016llx decoded=%u.%u.%u.%u result=%d\\n",
  p->raw_core_id,(unsigned long long)p->raw_bvnc,p->id.b,p->id.v,p->id.n,p->id.c,ret);
power_down:
 down=mfw_mfg_power_down(&io);
 if(down){p->faulted=true;if(!ret)ret=down;dev_err(dev,"Power-down stopped stage%u: %d; retaining clock\\n",io.stage,down);}
release_clock:
 if(clock_held&&!p->faulted&&mfw_probe_domains_off(p))clk_disable_unprepare(p->clock);
 else if(clock_held) p->faulted=true;
put_vcore:
 regulator_put(vcore);
unlock_pmic:
 device_unlock(&pmic->dev);put_device(&pmic->dev);
out:
 p->last_result=ret;
 mutex_unlock(&p->lock);
 return ret?ret:count;
}
static DEVICE_ATTR_RO(identity);
static DEVICE_ATTR_WO(probe_id);
static struct attribute *mfw_gpu_probe_attrs[]={&dev_attr_identity.attr,&dev_attr_probe_id.attr,NULL};
ATTRIBUTE_GROUPS(mfw_gpu_probe);
static int mfw_gpu_probe_init(struct platform_device *pdev)
{
 const resource_size_t bases[]={0x10006000,0x10001000,0x1020e000};
 struct device *dev=&pdev->dev;
 struct mfw_gpu_probe *p;
 unsigned i;
 if(!of_property_present(dev->of_node,"vcore-supply")||
    !of_property_present(dev->of_node,"microfw,pmic-provider"))return -EINVAL;
 p=devm_kzalloc(dev,sizeof(*p),GFP_KERNEL);if(!p)return -ENOMEM;
 p->dev=dev;p->last_result=-EAGAIN;mutex_init(&p->lock);
 for(i=0;i<3;i++){
  struct resource *res=platform_get_resource(pdev,IORESOURCE_MEM,i);
  if(!res||res->start!=bases[i]||resource_size(res)!=4096)return -EINVAL;
  p->blocks[i]=devm_ioremap(dev,res->start,resource_size(res));
  if(!p->blocks[i])return -ENOMEM;
 }
 p->clock=devm_clk_get(dev,NULL);
 if(IS_ERR(p->clock))return dev_err_probe(dev,PTR_ERR(p->clock),"MFG probe clock unavailable\\n");
 platform_set_drvdata(pdev,p);
 dev_info(dev,"GPU power/ID probe ready; idle until explicit sysfs request\\n");
 return 0;
}
static const struct of_device_id mfw_gpu_probe_match[]={
 {.compatible="microfw,mt6739-gpu-id-probe"},{}
};
MODULE_DEVICE_TABLE(of,mfw_gpu_probe_match);
static struct platform_driver mfw_gpu_probe_driver={
 .probe=mfw_gpu_probe_init,
 .driver={.name="mfw-gpu-id-probe",.of_match_table=mfw_gpu_probe_match,
  .dev_groups=mfw_gpu_probe_groups,.suppress_bind_attrs=true},
};
module_platform_driver(mfw_gpu_probe_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MICROFW bounded26MHz MT6739 GPU identity probe, not acceleration");
''',newline='\n')
(w/'compile_gpu_id_probe.sh').write_text('''#!/bin/bash
set -eu
cd /home/y5/linux-7.3
mkdir -p drivers/gpu/drm/mfw-gpu-probe
cp /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id.h /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-power-sequence.h drivers/gpu/drm/mfw-gpu-probe/
printf 'obj-y += mfw-gpu-id-probe.o\\n' > drivers/gpu/drm/mfw-gpu-probe/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/gpu/drm/mfw-gpu-probe/mfw-gpu-id-probe.o
cd /mnt/d/y5_linux_work/microfw-gpu
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_gpu_id.c -o /tmp/mfw-test-gpu-id
/tmp/mfw-test-gpu-id
''',newline='\n')
