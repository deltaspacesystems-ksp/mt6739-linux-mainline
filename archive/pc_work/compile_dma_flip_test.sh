#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cp /mnt/d/y5_linux_work/microfw-display/mfw-rdma-dma-test.c drivers/gpu/drm/mfw-bringup/mfw-rdma-dma-test.c
cp /mnt/d/y5_linux_work/microfw-display/mfw-frame-switch.h drivers/gpu/drm/mfw-bringup/
printf 'obj-y += mfw-rdma-dma-test.o\n' > drivers/gpu/drm/mfw-bringup/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/gpu/drm/mfw-bringup/mfw-rdma-dma-test.o
cd /mnt/d/y5_linux_work/microfw-display
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_frame_switch.c -o /tmp/mfw-test-frame-switch
/tmp/mfw-test-frame-switch
