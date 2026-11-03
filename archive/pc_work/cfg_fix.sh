cd /home/y5/linux-7.3
cp .config.old .config
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig >/dev/null 2>&1
grep -cE "^CONFIG_ARM64=y|^CONFIG_ARCH_MEDIATEK=y" .config
grep -E "^CONFIG_(MUSB_PIO_ONLY|DEBUG_FS|MMC_MTK|USB_MUSB_MEDIATEK|PHY_MTK_TPHY|MEDIATEK_WATCHDOG|DEVMEM|STRICT_DEVMEM|USB_ETH|USB_MUSB_GADGET)[= ]" .config; grep "STRICT_DEVMEM" .config
rm -f /home/y5/build73f.log
nohup bash /mnt/d/y5_linux_work/bld.sh >/dev/null 2>&1 &
sleep 2; echo started
