cd /home/y5/linux-7.3
echo "-- early_param initrd:"; grep -rn 'early_param("initrd"' arch/arm64 arch/arm init drivers/of 2>/dev/null | head -3
echo "-- phys_initrd arm64:"; grep -n 'phys_initrd_start' arch/arm64/mm/init.c | head -3
echo "-- config:"; grep -E 'CONFIG_(BLK_DEV_INITRD|INITRAMFS_SOURCE|RD_GZIP|BLK_DEV_RAM|DEFAULT_INIT|PANIC_TIMEOUT)=' .config
echo "-- of initrd:"; grep -n 'linux,initrd' drivers/of/fdt.c | head -3
echo "-- build script:"; grep -n -i 'initrd\|ramdisk\|ram_addr\|tags_addr\|dtb_addr' /mnt/d/y5_linux_work/build_mainline.py | head -14
