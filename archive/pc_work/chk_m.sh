cd /home/y5/linux-7.3
grep -n "ENODEV" drivers/usb/musb/musb_core.c | head -30
echo ---CFG
grep -E "CONFIG_(MMC_MTK|DEBUG_FS|MMC_BLOCK|USB_MUSB_MEDIATEK|MUSB_PIO_ONLY|USB_MUSB_DUAL_ROLE|USB_MUSB_GADGET|USB_MUSB_HOST|USB_G_ETHER|USB_ETH)=|CONFIG_MMC=" .config
echo ---MTKINIT
sed -n '/static int mtk_musb_init/,/^}/p' drivers/usb/musb/mediatek.c | head -70
