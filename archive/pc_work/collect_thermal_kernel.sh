#!/bin/bash
set -eu
cd /home/y5/linux-7.3
stat -c '%n %s %y' arch/arm64/boot/Image
strings vmlinux | grep 'VBAT + NTC + three buttons'
grep -qx 'CONFIG_ARM64=y' .config
grep -qx 'CONFIG_Y5_PMIC_MONITOR=y' .config
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_wd24
