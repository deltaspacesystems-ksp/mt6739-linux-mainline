#!/bin/sh
set -eu
umask 077
p=/tmp/mfw-usc-meta
mkdir "$p"
cleanup(){ umount "$p" 2>/dev/null || true;rmdir "$p"; }
trap cleanup EXIT
[ "$(sed -n 's/^PARTNAME=//p' /sys/class/block/mmcblk0p41/uevent)" = vendor ]
mount -t ext4 -o ro,noload /dev/mmcblk0p41 "$p"
readelf -Ws "$p/lib64/libusc.so" 2>/tmp/mfw-usc-warnings | awk '$4=="FUNC" && $7!="UND" {print $8}' | sort -u > /tmp/mfw-usc-defined-symbols
printf 'Defined compiler functions: ';wc -l < /tmp/mfw-usc-defined-symbols
grep -Ei 'core|feature|hardware|limit|tile|store|USC|UniFlex' /tmp/mfw-usc-defined-symbols | head -55
