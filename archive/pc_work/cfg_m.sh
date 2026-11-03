cd /home/y5/linux-7.3
scripts/config --enable MUSB_PIO_ONLY --enable DEBUG_FS
make olddefconfig >/dev/null 2>&1
grep -E "CONFIG_(MUSB_PIO_ONLY|USB_INVENTRA_DMA|DEBUG_FS|USB_UX500_DMA|USB_TI_CPPI41_DMA)[= ]" .config
cat /mnt/d/y5_linux_work/bld.sh
