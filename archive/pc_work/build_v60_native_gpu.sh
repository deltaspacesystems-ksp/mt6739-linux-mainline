#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/install_ddk19_kernel.py
cd /home/y5/linux-7.3
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig > /mnt/d/y5_linux_work/out/v60-olddefconfig.log 2>&1
grep -qx 'CONFIG_DRM_MFW_ROGUE_DDK19=y' .config
grep -qx 'CONFIG_ARM64=y' .config
touch /mnt/d/y5_linux_work/out/v60-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v60-build-start
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v60
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v60
cat include/generated/utsversion.h
tail -n 6 /home/y5/build73f.log
