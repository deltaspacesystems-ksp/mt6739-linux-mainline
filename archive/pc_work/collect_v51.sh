#!/bin/bash
set -eu
cd /home/y5/linux-7.3
tail -n 6 /home/y5/build73f.log
stat -c '%n %s %y' arch/arm64/boot/Image
strings vmlinux | grep -E 'read-only charge status|CPU nominal clock'
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v51
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v51
