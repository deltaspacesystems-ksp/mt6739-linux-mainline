from pathlib import Path
w=Path(r'D:\y5_linux_work');d=w/'microfw-gpu';d.mkdir(exist_ok=True)
(d/'mfw-mfg-clock-math.h').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_MFG_CLOCK_MATH_H
#define MFW_MFG_CLOCK_MATH_H
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif
/* MT6739 PLL21bit PCW,7integerbits ->14fractional; divider CON1[26:24].
 * This decodes registers, not a measured frequency counter result. */
static inline u64 mfw_mfg_pll_rate(u32 con0,u32 con1,u32 power){
 if(!(con0&1)||!(power&1)||(power&2))return 0;
 return ((26000000ULL*(con1&0x1fffff))>>14)>>((con1>>24)&7);
}
#endif
''',newline='\n')
(d/'clk-mfw-mfg-probe.c').write_text('''// SPDX-License-Identifier: GPL-2.0-only
/* Transitional MFG CCF clock for a26MHz identity probe, not DVFS support.
 * No PLL writes, voltage changes or GPU-core access. Registers/source:
 * Power535/android_kernel_common_MT6763@47f925eac07c660002cb85939c94cd8a191e1c53
 * drivers/clk/mediatek/clk-mt6739.c; matched MFG fields to stock4.4.
 * Do not bind alongside a proper MT6739 TOP/APMIXED CCF provider.
 */
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include "mfw-mfg-clock-math.h"

struct mfw_mfg_clock {
 struct clk_hw hw;
 void __iomem *top, *pll, *spm;
 spinlock_t lock;
};
#define to_mfw_clock(h) container_of(h, struct mfw_mfg_clock, hw)
static bool mfw_domains_off(struct mfw_mfg_clock *c)
{
 return !((readl(c->spm + 0x180) | readl(c->spm + 0x184)) & (BIT(4) | BIT(31)));
}
static unsigned long mfw_mfg_rate(struct clk_hw *hw, unsigned long parent_rate)
{
 struct mfw_mfg_clock *c = to_mfw_clock(hw);
 unsigned int parent = (readl(c->top + 0x50) >> 24) & 3;
 unsigned int offset = parent == 1 ? 0x240 : 0x220;
 u64 rate;
 if (!parent) return 26000000;
 rate = mfw_mfg_pll_rate(readl(c->pll + offset), readl(c->pll + offset + 4),
                         readl(c->pll + offset + 12));
 if (parent == 2) rate /= 3;
 if (parent == 3) rate /= 5;
 return rate;
}
static long mfw_mfg_round_rate(struct clk_hw *hw, unsigned long rate, unsigned long *parent_rate)
{
 return rate == 26000000 ? 26000000 : -EINVAL;
}
static int mfw_mfg_set_rate(struct clk_hw *hw, unsigned long rate, unsigned long parent_rate)
{
 struct mfw_mfg_clock *c = to_mfw_clock(hw);
 unsigned long flags;
 u32 value;
 int ret;
 if (rate != 26000000) return -EINVAL;
 spin_lock_irqsave(&c->lock, flags);
 if (!mfw_domains_off(c)) { ret = -EBUSY; goto out; }
 /* W1CLR only MFG selector[25:24]; no other clock field is modified. */
 writel(GENMASK(25, 24), c->top + 0x58);
 writel(BIT(7), c->top + 4);
 ret = readl_poll_timeout_atomic(c->top + 0x50, value, !(value & GENMASK(25, 24)), 1, 1000);
out:
 spin_unlock_irqrestore(&c->lock, flags);
 return ret;
}
static int mfw_mfg_enable(struct clk_hw *hw)
{
 struct mfw_mfg_clock *c = to_mfw_clock(hw);
 unsigned long flags;
 u32 value;
 int ret;
 spin_lock_irqsave(&c->lock, flags);
 if (readl(c->top + 0x50) & GENMASK(25, 24)) { ret = -EPERM; goto out; }
 writel(BIT(31), c->top + 0x58);
 writel(BIT(7), c->top + 4);
 ret = readl_poll_timeout_atomic(c->top + 0x50, value, !(value & BIT(31)), 1, 1000);
out:
 spin_unlock_irqrestore(&c->lock, flags);
 return ret;
}
static void mfw_mfg_disable(struct clk_hw *hw)
{
 struct mfw_mfg_clock *c = to_mfw_clock(hw);
 unsigned long flags;
 spin_lock_irqsave(&c->lock, flags);
 /* On partial power-transition failures keep SRAM handshake clocks alive. */
 if (mfw_domains_off(c)) {
  writel(BIT(31), c->top + 0x54);
  writel(BIT(7), c->top + 4);
 }
 spin_unlock_irqrestore(&c->lock, flags);
}
static int mfw_mfg_is_enabled(struct clk_hw *hw)
{
 return !(readl(to_mfw_clock(hw)->top + 0x50) & BIT(31));
}
static const struct clk_ops mfw_mfg_ops = {
 .recalc_rate = mfw_mfg_rate, .round_rate = mfw_mfg_round_rate,
 .set_rate = mfw_mfg_set_rate, .enable = mfw_mfg_enable,
 .disable = mfw_mfg_disable, .is_enabled = mfw_mfg_is_enabled,
};
static int mfw_mfg_clock_probe(struct platform_device *pdev)
{
 const resource_size_t bases[] = { 0x10000000, 0x1000c000, 0x10006000 };
 struct device *dev = &pdev->dev;
 struct mfw_mfg_clock *c;
 struct clk_init_data init = {
  .name = "mfw_mfg_probe_ck", .ops = &mfw_mfg_ops,
  .flags = CLK_IGNORE_UNUSED | CLK_GET_RATE_NOCACHE,
 };
 void __iomem **maps[3];
 unsigned int i;
 int ret;
 c = devm_kzalloc(dev, sizeof(*c), GFP_KERNEL);
 if (!c) return -ENOMEM;
 maps[0] = &c->top; maps[1] = &c->pll; maps[2] = &c->spm;
 for (i = 0; i < 3; i++) {
  struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, i);
  if (!res || res->start != bases[i] || resource_size(res) != 4096) return -EINVAL;
  /* Shared legacy always-on syscons. Only the MFG fields above are written. */
  *maps[i] = devm_ioremap(dev, res->start, resource_size(res));
  if (!*maps[i]) return -ENOMEM;
 }
 spin_lock_init(&c->lock); c->hw.init = &init;
 ret = devm_clk_hw_register(dev, &c->hw);
 if (ret) return ret;
 ret = devm_of_clk_add_hw_provider(dev, of_clk_hw_simple_get, &c->hw);
 if (!ret) dev_info(dev, "MFG CCF probe clock registered, rate=%lu; no automatic rate change\\n", clk_hw_get_rate(&c->hw));
 return ret;
}
static const struct of_device_id mfw_mfg_clock_match[] = {
 { .compatible = "microfw,mt6739-mfg-probe-clock" }, {}
};
MODULE_DEVICE_TABLE(of, mfw_mfg_clock_match);
static struct platform_driver mfw_mfg_clock_driver = {
 .probe = mfw_mfg_clock_probe,
 .driver = { .name = "mfw-mfg-probe-clock", .of_match_table = mfw_mfg_clock_match },
};
module_platform_driver(mfw_mfg_clock_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MICROFW MT6739 bounded MFG26MHz CCF probe clock");
''',newline='\n')
(d/'test_mfg_clock.c').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <stdio.h>
#include "mfw-mfg-clock-math.h"
int main(void){
 u64 rate=mfw_mfg_pll_rate(0x181,0x0215ec4e,0x80000001);
 assert(rate>=569999000&&rate<=570001000);
 assert(!mfw_mfg_pll_rate(0,0x0215ec4e,1));
 assert(!mfw_mfg_pll_rate(1,0x0215ec4e,0));
 assert(!mfw_mfg_pll_rate(1,0x0215ec4e,3));
 assert(mfw_mfg_pll_rate(1,1U<<14,1)==26000000);
 assert(mfw_mfg_pll_rate(1,(7U<<24)|(1U<<14),1)==203125);
 printf("PASS: MT6739 PLL decode/live register candidate=%lluHz, enable/isolation and divider boundaries\\n",(unsigned long long)rate);return 0;
}
''',newline='\n')
(w/'compile_mfg_clock.sh').write_text('''#!/bin/bash
set -eu
cd /home/y5/linux-7.3
mkdir -p drivers/clk/mediatek/mfw-bringup
cp /mnt/d/y5_linux_work/microfw-gpu/clk-mfw-mfg-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-clock-math.h drivers/clk/mediatek/mfw-bringup/
printf 'obj-y += clk-mfw-mfg-probe.o\\n' > drivers/clk/mediatek/mfw-bringup/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/clk/mediatek/mfw-bringup/clk-mfw-mfg-probe.o
cd /mnt/d/y5_linux_work/microfw-gpu
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_mfg_clock.c -o /tmp/mfw-test-mfg-clock
/tmp/mfw-test-mfg-clock
''',newline='\n')
print('Prepared CCF MFG probe clock source; compile only, not installed in any Image/DT')
