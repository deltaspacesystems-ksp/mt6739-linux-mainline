#!/bin/bash
set -eu
cd /home/y5/linux-7.3
tail -n 5 /home/y5/build73f.log
strings vmlinux | grep 'DURA-L21: TOPRGU restart priority 192'
stat -c '%n %s %y' arch/arm64/boot/Image
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v52
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v51
