K='/home/y5/linux-7.3/'
s=open(K+'init/main.c').read()
a='void __iomem *y5_wdt_base;'
assert s.count(a)==1
hack='''
static int __init y5_clk_hack(void)
{
	/* ungate zegarow USB: infracfg infra_icusb (CLR 0x84 bit 8), topckgen usb_top_sel (CLR 0xb8 bit 31 + UPD 0x4 bit 30) */
	void __iomem *infra = ioremap(0x10001000, 0x1000);
	void __iomem *top = ioremap(0x10000000, 0x1000);
	if (infra) { writel(BIT(8), infra + 0x84); iounmap(infra); }
	if (top) { writel(BIT(31), top + 0xb8); writel(BIT(30), top + 0x4); iounmap(top); }
	return 0;
}
early_initcall(y5_clk_hack);
'''
s=s.replace(a,a+'\n'+hack,1)
open(K+'init/main.c','w').write(s)
print('ok')
