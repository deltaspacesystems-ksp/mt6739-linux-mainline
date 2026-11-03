#!/bin/bash
set -eu
make -C /home/y5/linux-7.3 ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- M=/mnt/d/y5_linux_work/mfw-i2c1-probe W=1 KCFLAGS=-Werror modules
