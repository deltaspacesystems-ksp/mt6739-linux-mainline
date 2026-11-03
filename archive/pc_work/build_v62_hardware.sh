#!/bin/bash
set -eu
cd /home/y5/linux-7.3
scripts/config --enable MODULES --enable MODULE_UNLOAD --enable HOTPLUG_CPU --enable KEXEC_FILE --disable KEXEC_HANDOVER --disable CRASH_DUMP
scripts/config --module RFKILL --module CFG80211 --module MAC80211 --module BT --enable BT_BREDR --enable BT_LE --module BT_RFCOMM --module BT_BNEP --module BT_HIDP --module WWAN --module GNSS
scripts/config --module I2C_MT65XX --module IIO --enable IIO_BUFFER --enable IIO_TRIGGER --module MC3230
scripts/config --module SOUND --module SND --module SND_SOC --module SND_SOC_MT6357
scripts/config --module MEDIA_SUPPORT --enable MEDIA_CAMERA_SUPPORT --enable MEDIA_CONTROLLER --module VIDEO_DEV --module V4L2_FWNODE --module VIDEO_HI556 --module VIDEO_HI846 --module VIDEO_OV8856
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig > /mnt/d/y5_linux_work/out/v62-olddefconfig.log 2>&1
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v62
scripts/diffconfig /mnt/d/y5_linux_work/export/config-7.3-mfw-v61 .config > /mnt/d/y5_linux_work/out/v62-config-delta.txt
for symbol in CONFIG_KEXEC_FILE=y CONFIG_MODULES=y CONFIG_HOTPLUG_CPU=y CONFIG_DRM_MFW_ROGUE_DDK19=y; do grep -q "^$symbol$" .config; done
touch /mnt/d/y5_linux_work/out/v62-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v62-build-start
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v62
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j6 modules > /mnt/d/y5_linux_work/out/v62-modules-build.log 2>&1
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- INSTALL_MOD_PATH=/mnt/d/y5_linux_work/out/v62-module-root modules_install > /mnt/d/y5_linux_work/out/v62-modules-install.log 2>&1
cat include/generated/utsrelease.h include/generated/utsversion.h
stat -c '%s bytes Image' arch/arm64/boot/Image
