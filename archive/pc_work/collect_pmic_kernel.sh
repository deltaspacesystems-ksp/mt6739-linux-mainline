#!/bin/bash
set -eu
cd /home/y5/linux-7.3
stat -c '%n %s %y' arch/arm64/boot/Image
nm vmlinux | grep -q ' t y5_pmic_probe$'
grep -qx 'CONFIG_ARM64=y' .config
grep -qx 'CONFIG_Y5_PMIC_MONITOR=y' .config
grep -qx 'CONFIG_INPUT_EVDEV=y' .config
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_wd23
cp .config /mnt/d/y5_linux_work/export/config-7.3-y5-pmic