#!/bin/bash
set -eu
cd /home/y5/linux-7.3
# Compile in an isolated directory, preserving the linked production source.
mkdir -p drivers/power/supply/mfw-bringup
cp /mnt/d/y5_linux_work/export/y5-pmic-v59.c drivers/power/supply/mfw-bringup/y5-pmic-v59.c
cp drivers/power/supply/y5-ntc.h drivers/power/supply/mfw-charger-state.h drivers/power/supply/mfw-hl-policy.h drivers/power/supply/mfw-bringup/
printf 'obj-y += y5-pmic-v59.o\n' > drivers/power/supply/mfw-bringup/Makefile
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- W=1 drivers/power/supply/mfw-bringup/y5-pmic-v59.o
