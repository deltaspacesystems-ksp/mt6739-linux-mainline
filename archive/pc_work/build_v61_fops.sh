#!/bin/bash
set -eu
python3 /mnt/d/y5_linux_work/fix_v61_fops.py
cd /home/y5/linux-7.3
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v60
touch /mnt/d/y5_linux_work/out/v61-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v61-build-start
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v61
cat include/generated/utsversion.h
tail -n 5 /home/y5/build73f.log
