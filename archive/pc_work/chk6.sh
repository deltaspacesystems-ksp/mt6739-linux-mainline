cd /home/y5/linux-7.3/drivers/phy/mediatek
grep -n 'sif_base' phy-mtk-tphy.c | head -20
sed -n '/^static int mtk_tphy_probe/,/^}/p' phy-mtk-tphy.c | head -75
sed -n '/^static void phy_v1_banks_init/,/^}/p' phy-mtk-tphy.c
