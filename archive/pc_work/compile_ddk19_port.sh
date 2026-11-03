#!/bin/bash
set -eu
# Individual objects only: no modules, config changes, linking or installation.
cd /home/y5/linux-7.3
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- \
  M=/mnt/d/y5_linux_work/rogue-ddk19-port MTK_PLATFORM=mt6739 \
  W=1 "${@:-kernel/drivers/staging/imgtec/pvr_drm.o}" > /mnt/d/y5_linux_work/out/ddk19-compile.log 2>&1
