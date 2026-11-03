#!/bin/bash
set -eu
cd /home/y5/linux-7.3
mkdir -p drivers/gpu/drm/mfw-bringup
cp /mnt/d/y5_linux_work/microfw-display/mfw-rdma-observer.c drivers/gpu/drm/mfw-bringup/
printf 'obj-y += mfw-rdma-observer.o\n' > drivers/gpu/drm/mfw-bringup/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/gpu/drm/mfw-bringup/mfw-rdma-observer.o
