// SPDX-License-Identifier: GPL-2.0-only
/* Read-only CCF provider for the LK-initialised MT6739 CPU clock.
 * No PLL programming, rate switching, regulator control or CPUFreq policy.
 */
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include "mfw-cpu-rate.h"

struct mfw_cpu_clock {
	struct clk_hw hw;
	struct regmap *apmixed, *ao;
	struct clk *xtal;
};

static unsigned long mfw_cpu_recalc(struct clk_hw *hw, unsigned long parent)
{
	struct mfw_cpu_clock *c = container_of(hw, struct mfw_cpu_clock, hw);
	u32 pll, div, mux, check_pll, check_mux;
	if (regmap_read(c->apmixed, 0x204, &pll) ||
	    regmap_read(c->ao, 0, &mux) || regmap_read(c->ao, 0x2c, &div) ||
	    regmap_read(c->apmixed, 0x204, &check_pll) ||
	    regmap_read(c->ao, 0, &check_mux) ||
	    pll != check_pll || mux != check_mux)
		return 0;
	return mfw_cpu_rate(pll, div, mux, parent);
}

static const struct clk_ops mfw_cpu_ops = { .recalc_rate = mfw_cpu_recalc };

static ssize_t cpu_rate_hz_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct mfw_cpu_clock *c = dev_get_drvdata(dev);
	unsigned long rate = mfw_cpu_recalc(&c->hw, clk_get_rate(c->xtal));
	return rate ? sysfs_emit(buf, "%lu\n", rate) : -ENODATA;
}
static DEVICE_ATTR_RO(cpu_rate_hz);
static struct attribute *mfw_cpu_attrs[] = { &dev_attr_cpu_rate_hz.attr, NULL };
ATTRIBUTE_GROUPS(mfw_cpu);

static int mfw_cpu_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	static const struct clk_parent_data parent = { .fw_name = "xtal" };
	struct clk_init_data init = {
		.name = "mfw_cpu", .ops = &mfw_cpu_ops,
		.parent_data = &parent, .num_parents = 1,
		.flags = CLK_GET_RATE_NOCACHE,
	};
	struct mfw_cpu_clock *c;
	unsigned long rate;
	int ret;
	c = devm_kzalloc(dev, sizeof(*c), GFP_KERNEL);
	if (!c)
		return -ENOMEM;
	c->apmixed = syscon_regmap_lookup_by_phandle(dev->of_node, "mediatek,apmixedsys");
	if (IS_ERR(c->apmixed))
		return dev_err_probe(dev, PTR_ERR(c->apmixed), "APMIXED unavailable\n");
	c->ao = syscon_regmap_lookup_by_phandle(dev->of_node, "mediatek,topckgen-ao");
	if (IS_ERR(c->ao))
		return dev_err_probe(dev, PTR_ERR(c->ao), "TOPCKGEN_AO unavailable\n");
	c->xtal = devm_clk_get(dev, "xtal");
	if (IS_ERR(c->xtal))
		return dev_err_probe(dev, PTR_ERR(c->xtal), "reference clock unavailable\n");
	rate = mfw_cpu_recalc(&c->hw, clk_get_rate(c->xtal));
	if (!rate)
		return dev_err_probe(dev, -ENODATA, "unsupported or changing CPU clock path\n");
	c->hw.init = &init;
	platform_set_drvdata(pdev, c);
	ret = devm_clk_hw_register(dev, &c->hw);
	if (ret)
		return ret;
	ret = devm_of_clk_add_hw_provider(dev, of_clk_hw_simple_get, &c->hw);
	if (!ret)
		dev_info(dev, "CPU nominal clock %lu Hz from PLL; read-only, no DVFS\n", rate);
	return ret;
}

static const struct of_device_id mfw_cpu_match[] = {
	{ .compatible = "microfw,mt6739-boot-cpu-clock" }, {}
};
MODULE_DEVICE_TABLE(of, mfw_cpu_match);
static struct platform_driver mfw_cpu_driver = {
	.probe = mfw_cpu_probe,
	.driver = {
		.name = "mfw-cpu-clock", .of_match_table = mfw_cpu_match,
		.dev_groups = mfw_cpu_groups,
	},
};
module_platform_driver(mfw_cpu_driver);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MT6739 read-only boot CPU clock provider");
