#!/bin/sh
set -eu
umask 077
base=/run/mfw-gpu-libs-readonly
mkdir "$base"
cleanup() {
 if grep -q " $base " /proc/mounts; then umount "$base"; fi
 rmdir "$base"
}
trap cleanup EXIT
test "$(sed -n 's/^PARTNAME=//p' /sys/class/block/mmcblk0p41/uevent)" = vendor
mount -t ext4 -o ro,noload /dev/mmcblk0p41 "$base"
for p in "$base"/lib/egl/lib*mtk*.so "$base"/lib64/egl/lib*mtk*.so "$base"/lib/libIMGegl.so "$base"/lib64/libIMGegl.so; do
 [ -f "$p" ] || continue
 echo "GPU_LIBRARY=${p#"$base"}"
 echo ELF_HEADER_FIRST20
 od -An -tx1 -N20 "$p"
 if command -v readelf >/dev/null 2>&1; then readelf -d "$p" | grep NEEDED || true; fi
done
