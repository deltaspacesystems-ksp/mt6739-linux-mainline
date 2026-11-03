import re
K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
rw('kernel/panic.c',lambda s:s.replace('y5_cp_regs(y5_wdt_base, 2)','y5_cp_regs(y5_wdt_base, 31)'))
rw('init/main.c',lambda s:s.replace('early_initcall(y5_clk_hack);','early_initcall(y5_clk_hack);\n',1).replace('	if (top) { writel(BIT(31), top + 0xb8); writel(BIT(30), top + 0x4); iounmap(top); }\n','	if (top) { writel(BIT(31), top + 0xb8); writel(BIT(30), top + 0x4); iounmap(top); }\n	if (y5_wdt_base)\n		y5_cp_regs(y5_wdt_base, 2);\n',1))
def dd(s):
    helper='''
extern void __iomem *y5_wdt_base;
static void y5_probe_cp(struct device *dev, int after)
{
	const char *n = dev_name(dev);
	u32 t = 0;

	if (!y5_wdt_base || !n)
		return;
	if (!strncmp(n, "11cc0000", 8))
		t = after ? 6 : 4;
	else if (!strncmp(n, "11200000", 8))
		t = after ? 10 : 8;
	else if (!strncmp(n, "11230000", 8))
		t = after ? 14 : 12;
	if (t)
		y5_cp_regs(y5_wdt_base, t);
}
'''
    a='static int really_probe(struct device *dev, const struct device_driver *drv, bool allow_async)'
    if a not in s:
        a='static int really_probe(struct device *dev, const struct device_driver *drv)'
    assert s.count(a)==1, 'really_probe sig'
    s=s.replace(a,helper+'\n'+a,1)
    b='	ret = call_driver_probe(dev, drv);'
    assert s.count(b)==1,'call_driver_probe'
    s=s.replace(b,'	y5_probe_cp(dev, 0);\n'+b+'\n	y5_probe_cp(dev, 1);',1)
    s=s.replace('#include <linux/device.h>','#include <linux/device.h>\n#include <linux/y5cp.h>',1)
    return s
rw('drivers/base/dd.c',dd)
print('ok')
