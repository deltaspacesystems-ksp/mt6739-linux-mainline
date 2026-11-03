#!/bin/bash
set -eu
cd /home/y5/linux-7.3
tail -n 5 /home/y5/build73f.log
strings vmlinux | grep 'HL7005D: watchdog service 8s'
cat include/generated/utsversion.h
stat -c '%n %s %y' arch/arm64/boot/Image
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v51
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v53