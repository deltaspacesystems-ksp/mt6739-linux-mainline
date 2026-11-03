#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cp /mnt/d/y5_linux_work/y5-pmic-monitor.c drivers/power/supply/y5-pmic-monitor.c
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j2 drivers/power/supply/y5-pmic-monitor.o > /mnt/d/y5_linux_work/out/pmic-compile.log 2>&1
nohup bash /mnt/d/y5_linux_work/bld.sh >/dev/null 2>&1 < /dev/null &
build_pid=$!
echo "$build_pid" > /mnt/d/y5_linux_work/out/pmic-build.pid
wait "$build_pid"