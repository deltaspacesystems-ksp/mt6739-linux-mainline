#!/bin/bash
set -eu
cd /home/y5/linux-7.3
mkdir -p drivers/clk/mediatek/mfw-bringup
cp /mnt/d/y5_linux_work/microfw-gpu/clk-mfw-mfg-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-clock-math.h drivers/clk/mediatek/mfw-bringup/
printf 'obj-y += clk-mfw-mfg-probe.o\n' > drivers/clk/mediatek/mfw-bringup/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/clk/mediatek/mfw-bringup/clk-mfw-mfg-probe.o
cd /mnt/d/y5_linux_work/microfw-gpu
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_mfg_clock.c -o /tmp/mfw-test-mfg-clock
/tmp/mfw-test-mfg-clock
