#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v57
cmp drivers/video/y5disp.c /mnt/d/y5_linux_work/export/y5disp-v56.c
cp /mnt/d/y5_linux_work/microfw-display/mfw-rdma-dma-test.c drivers/video/mfw-rdma-observer.c
cp /mnt/d/y5_linux_work/microfw-display/mfw-frame-switch.h drivers/video/
cp drivers/video/Makefile /mnt/d/y5_linux_work/export/video-Makefile-before-v58
if ! grep -q '^obj-y += mfw-rdma-observer.o$' drivers/video/Makefile; then
 printf '\n# MICROFW transitional IRQ observation, no modeset\nobj-y += mfw-rdma-observer.o\n' >> drivers/video/Makefile
fi
touch /mnt/d/y5_linux_work/out/v58-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v58-build-start
grep -q 'LK scanout observer IRQ=' drivers/video/mfw-rdma-observer.c
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v58
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v58
cp drivers/video/Makefile /mnt/d/y5_linux_work/export/video-Makefile-v58
cat include/generated/utsversion.h
tail -n 5 /home/y5/build73f.log
