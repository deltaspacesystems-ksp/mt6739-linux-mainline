#!/bin/sh
# Static ELF metadata only; never execute vendor libraries or load Bionic/HAL.
set -eu
umask 077
base=/tmp/mfw-gpu-library-readonly
command -v readelf >/dev/null
test "$(sed -n 's/^PARTNAME=//p' /sys/class/block/mmcblk0p41/uevent)" = vendor
mkdir "$base"
cleanup() {
 if grep -q " $base " /proc/mounts; then umount "$base"; fi
 rmdir "$base"
}
trap cleanup EXIT
mount -t ext4 -o ro,noload /dev/mmcblk0p41 "$base"
for name in libusc.so libsrv_um.so libglslcompiler.so; do
 p=$base/lib64/$name
 [ -f "$p" ] || continue
 [ ! -L "$p" ] || continue
 printf 'GENERIC_GPU_LIBRARY=%s BYTES=%s\n' "$name" "$(stat -c %s "$p")"
 readelf -Ws "$p" 2> /tmp/mfw-gpu-elf-warnings | awk '$8 ~ /PVR|UniFlex|USC|BVNC|Core|Feature|GPU/ {print $8}' | sort -u | head -20
done
