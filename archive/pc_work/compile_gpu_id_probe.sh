#!/bin/bash
set -eu
cd /home/y5/linux-7.3
mkdir -p drivers/gpu/drm/mfw-gpu-probe
cp /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id.h /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-power-sequence.h drivers/gpu/drm/mfw-gpu-probe/
printf 'obj-y += mfw-gpu-id-probe.o\n' > drivers/gpu/drm/mfw-gpu-probe/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/gpu/drm/mfw-gpu-probe/mfw-gpu-id-probe.o
cd /mnt/d/y5_linux_work/microfw-gpu
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_gpu_id.c -o /tmp/mfw-test-gpu-id
/tmp/mfw-test-gpu-id
