cd /home/y5/linux-7.3
scripts/config --enable REGULATOR_FIXED_VOLTAGE
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig >/dev/null 2>&1
grep -cE "^CONFIG_ARM64=y" .config
grep -E "^CONFIG_REGULATOR_FIXED_VOLTAGE|^CONFIG_MUSB_PIO_ONLY|^CONFIG_DEBUG_FS=" .config
rm -f /home/y5/build73f.log
nohup bash /mnt/d/y5_linux_work/bld.sh >/dev/null 2>&1 &
sleep 1; echo started
