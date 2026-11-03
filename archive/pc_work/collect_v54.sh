#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -qx CONFIG_ARM64=y .config
grep -qx CONFIG_INPUT_MISC=y .config
grep -qx CONFIG_INPUT_UINPUT=y .config
grep -qx CONFIG_I2C_CHARDEV=y .config
cat include/generated/utsversion.h
tail -n 4 /home/y5/build73f.log
stat -c '%n %s %y' arch/arm64/boot/Image
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v54
