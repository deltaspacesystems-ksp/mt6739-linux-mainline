#!/bin/bash
set -eu
cd /home/y5/linux-7.3
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- \
  M=/home/y5/rogue-ddk19-port MTK_PLATFORM=mt6739 KBUILD_BUILTIN=1 \
  W=1 -k -j2 "${@:-built-in.a}" > /mnt/d/y5_linux_work/out/ddk19-wsl.log 2>&1
