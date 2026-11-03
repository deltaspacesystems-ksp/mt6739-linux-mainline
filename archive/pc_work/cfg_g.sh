cd /home/y5/linux-7.3
scripts/config --disable USB_ETH --enable CONFIGFS_FS --enable USB_CONFIGFS --enable USB_CONFIGFS_RNDIS --enable USB_CONFIGFS_ECM --enable USB_CONFIGFS_NCM --enable USB_CONFIGFS_ACM --enable USB_CONFIGFS_SERIAL
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig >/dev/null 2>&1
grep -cE "^CONFIG_ARM64=y" .config
grep -E "^CONFIG_(USB_ETH|CONFIGFS_FS|USB_CONFIGFS|USB_CONFIGFS_RNDIS|USB_CONFIGFS_ECM|USB_CONFIGFS_NCM|USB_CONFIGFS_ACM|USB_LIBCOMPOSITE|USB_F_RNDIS|REGULATOR_FIXED_VOLTAGE|MUSB_PIO_ONLY)[= ]" .config
rm -f /home/y5/build73f.log
nohup bash /mnt/d/y5_linux_work/bld.sh >/dev/null 2>&1 &
sleep 1; echo started
