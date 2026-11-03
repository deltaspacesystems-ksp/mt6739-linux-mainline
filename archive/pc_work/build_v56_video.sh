#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -qx CONFIG_ARM64=y .config
grep -qx CONFIG_INPUT_UINPUT=y .config
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v54
cp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp-before-v56.c
cmp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp.c
cp /mnt/d/y5_linux_work/export/y5disp-v56.c drivers/video/y5disp.c
touch /mnt/d/y5_linux_work/out/v56-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v56-build-start
grep -Fq 'continuous video scanout; manual kicks disabled' drivers/video/y5disp.c
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v56
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v56
cat include/generated/utsversion.h
tail -n 5 /home/y5/build73f.log
