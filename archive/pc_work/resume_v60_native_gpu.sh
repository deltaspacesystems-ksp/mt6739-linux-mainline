#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -qx 'CONFIG_DRM_MFW_ROGUE_DDK19=y' .config
for relative in services/server/common/pmr.c services/server/common/pvrsrv.c; do
 cp /mnt/d/y5_linux_work/rogue-ddk19-port/$relative drivers/gpu/drm/mfw-rogue-ddk19/$relative
done
touch /mnt/d/y5_linux_work/out/v60-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v60-build-start
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v60
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v60
cat include/generated/utsversion.h
tail -n 6 /home/y5/build73f.log
