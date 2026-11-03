K='/home/y5/linux-7.3/'
def rw(p,f):
    s=open(K+p).read(); n=f(s); open(K+p,'w').write(n)
# 1) hack: test odczytu/zapisu MAC po sekwencji PHY
def mn(s):
    a='	if (y5_wdt_base)\n		y5_cp_regs(y5_wdt_base, 3);\n	return 0;\n}\nearly_initcall(y5_clk_hack);'
    assert s.count(a)==1
    b='''	if (y5_wdt_base)
		y5_cp_regs(y5_wdt_base, 3);
	{
		void __iomem *m = ioremap(0x11200000, 0x1000);

		if (m) {
			(void)readb(m + 0x0);
			if (y5_wdt_base)
				y5_cp_regs(y5_wdt_base, 5);
			writew(0x0, m + 0x82);
			if (y5_wdt_base)
				y5_cp_regs(y5_wdt_base, 7);
			iounmap(m);
		}
	}
	return 0;
}
early_initcall(y5_clk_hack);'''
    return s.replace(a,b,1)
rw('init/main.c',mn)
# 2) glue: strzaly w mtk_musb_init
def gl(s):
    s=s.replace('#include <linux/clk.h>','#include <linux/clk.h>\n#include <linux/y5cp.h>\nextern void __iomem *y5_wdt_base;\n#define Y5A(t) do { if (y5_wdt_base) y5_cp_regs(y5_wdt_base, (t)); } while (0)',1)
    a='	glue->musb = musb;\n	musb->phy = glue->phy;'
    assert s.count(a)==1
    s=s.replace(a,'	Y5A(9);\n'+a,1)
    b='	if (musb->port_mode == MUSB_OTG) {\n		ret = mtk_otg_switch_init(glue);'
    assert s.count(b)==1
    s=s.replace(b,'	Y5A(11);\n'+b,1)
    c='	ret = phy_power_on(glue->phy);\n	if (ret)\n		goto err_phy_power_on;\n'
    assert s.count(c)==1
    s=s.replace(c,'	Y5A(13);\n'+c+'	Y5A(15);\n',1)
    d='	musb_writel(musb->mregs, USB_L1INTM, TX_INT_STATUS | RX_INT_STATUS |\n		    USBCOM_INT_STATUS | DMA_INT_STATUS);\n	return 0;'
    assert s.count(d)==1
    s=s.replace(d,d.replace('	return 0;','	Y5A(17);\n	return 0;'),1)
    return s
rw('drivers/usb/musb/mediatek.c',gl)
print('ok')
