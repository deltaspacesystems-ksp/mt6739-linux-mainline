cd /home/y5/linux-7.3
sed -n '/^static int mtk_musb_init/,/^}/p' drivers/usb/musb/mediatek.c
grep -n 'mtk_musb_clks_enable\|^static int mtk_musb_clks' drivers/usb/musb/mediatek.c | head
sed -n '/^static int mtk_phy_init/,/^}/p' drivers/phy/mediatek/phy-mtk-tphy.c | head -50
