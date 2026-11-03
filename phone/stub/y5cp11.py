K='/home/y5/linux-7.3/'
s=open(K+'init/main.c').read()
a='	if (y5_wdt_base)\n		y5_cp_regs(y5_wdt_base, 2);\n	return 0;\n}\nearly_initcall(y5_clk_hack);'
assert s.count(a)==1
new='''	if (y5_wdt_base)
		y5_cp_regs(y5_wdt_base, 2);
	{
		/* sekwencja usb_phy_recover() z 4.4 (PHY @0x11cc0000), przed sondowaniem musb */
		void __iomem *p = ioremap(0x11cc0000, 0x1000);
		u32 v, f, c, r1;
		int to = 100000;

		if (p) {
#define Y5RM(o, andm, orm) writel((readl(p + (o)) & (andm)) | (orm), p + (o))
			udelay(50);
			Y5RM(0x81c, 0xffffefff, 0);
			Y5RM(0x868, 0xfbffffff, 0);
			Y5RM(0x86c, 0xfffeffff, 0);
			Y5RM(0x820, 0xfffffdff, 0);
			Y5RM(0x820, 0xfffffeff, 0);
			Y5RM(0x868, 0xfffbffff, 0);
			Y5RM(0x868, 0xffffff3f, 0);
			Y5RM(0x868, 0xffffffcf, 0);
			Y5RM(0x868, 0xfffffffb, 0);
			Y5RM(0x868, 0xffffc3ff, 0);
			Y5RM(0x868, 0xff45ffff, 0);
			Y5RM(0x818, 0xff7fffff, 0);
			mdelay(5);
			Y5RM(0x814, 0xffffffff, 0x8000);
			udelay(1);
			Y5RM(0xf10, 0xffffffff, 0x100);
			Y5RM(0xf00, 0xffffffff, 0x400);
			Y5RM(0xf00, 0xffffffff, 0x1000000);
			while ((readl(p + 0xf10) & 0xff) != 1 && --to)
				udelay(1);
			f = readl(p + 0xf0c);
			c = f ? 0x150000 / f : 0;
			v = (c / 1000) & 0xff;
			r1 = (v + 1) & 0xff;
			if (c - v * 1000 < 500)
				r1 = v;
			Y5RM(0xf00, 0xfeffffff, 0);
			Y5RM(0xf10, 0xfffffeff, 0);
			Y5RM(0x814, 0xffff8fff, 0);
			Y5RM(0x814, 0xffffffff, (r1 & 7) << 12);
			Y5RM(0x814, 0xffff7fff, 0);
			Y5RM(0x818, 0xffffffff, 0xf0);
			Y5RM(0x818, 0x3fffffff, 0);
			Y5RM(0x818, 0xffffffff, 0xc0000000);
			udelay(5);
			iounmap(p);
		}
	}
	if (y5_wdt_base)
		y5_cp_regs(y5_wdt_base, 3);
	return 0;
}
early_initcall(y5_clk_hack);'''
s=s.replace(a,new,1)
if '#include <linux/delay.h>' not in s:
    s=s.replace('#include <linux/y5cp.h>','#include <linux/y5cp.h>\n#include <linux/delay.h>',1)
open(K+'init/main.c','w').write(s)
print('ok')
