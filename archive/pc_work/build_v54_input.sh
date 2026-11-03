#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cp .config /mnt/d/y5_linux_work/export/config-before-v54
scripts/config --enable INPUT_MISC --enable INPUT_UINPUT --enable I2C_CHARDEV
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig > /mnt/d/y5_linux_work/out/v54-config.log 2>&1
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v54
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
