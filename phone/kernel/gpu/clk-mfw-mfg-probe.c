// SPDX-License-Identifier: GPL-2.0-only
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
#ifdef MODULE
#error "MFG probe clock must remain built-in"
#endif
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
static int mfw_mfg_determine_rate(struct clk_hw *hw, struct clk_rate_request *req)
{
 if (req->rate != 26000000 || req->min_rate > 26000000 || req->max_rate < 26000000)
  return -EINVAL;
 return 0;
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
 .recalc_rate = mfw_mfg_rate, .determine_rate = mfw_mfg_determine_rate,
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
 if (!ret) dev_info(dev, "MFG CCF probe clock registered, rate=%lu; no automatic rate change\n", clk_hw_get_rate(&c->hw));
 return ret;
}
static const struct of_device_id mfw_mfg_clock_match[] = {
 { .compatible = "microfw,mt6739-mfg-probe-clock" }, {}
};
MODULE_DEVICE_TABLE(of, mfw_mfg_clock_match);
static struct platform_driver mfw_mfg_clock_driver = {
 .probe = mfw_mfg_clock_probe,
 .driver = { .name = "mfw-mfg-probe-clock", .of_match_table = mfw_mfg_clock_match, .suppress_bind_attrs = true },
};
module_platform_driver(mfw_mfg_clock_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MICROFW MT6739 bounded MFG26MHz CCF probe clock");
