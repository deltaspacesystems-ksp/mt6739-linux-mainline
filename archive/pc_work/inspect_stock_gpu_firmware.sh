#!/bin/sh
set -eu
umask 077
base=/run/mfw-fw-readonly
mkdir "$base"
cleanup() {
 for n in vendor system; do
  if grep -q " $base/$n " /proc/mounts; then umount "$base/$n"; fi
  rmdir "$base/$n" 2>/dev/null || true
 done
 rmdir "$base"
}
trap cleanup EXIT
for item in vendor:41 system:42; do
 name=${item%:*}; number=${item#*:}
 test "$(sed -n 's/^PARTNAME=//p' /sys/class/block/mmcblk0p"$number"/uevent)" = "$name"
 mkdir "$base/$name"
 mount -t ext4 -o ro,noload /dev/mmcblk0p"$number" "$base/$name"
 # Deliberately inspect only generic GPU firmware names, no user data/config.
 for p in "$base/$name"/firmware/rgx* "$base/$name"/etc/firmware/rgx* "$base/$name"/lib/firmware/rgx* "$base/$name"/vendor/firmware/rgx* "$base/$name"/vendor/etc/firmware/rgx*; do
  [ -f "$p" ] || continue
  echo "GPU_FIRMWARE_FILE=$p"
  wc -c < "$p"
  sha256sum "$p"
  echo HEADER_FIRST16_HEX
  od -An -tx1 -N16 "$p"
  bytes=$(stat -c %s "$p")
  if [ "$bytes" -gt 4096 ]; then
   echo MAINLINE_TRAILER_CANDIDATE_HEX
   od -An -tx1 -j "$((bytes-4096))" -N64 "$p"
  fi
 done
done
