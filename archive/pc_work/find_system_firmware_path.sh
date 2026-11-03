#!/bin/sh
for p in /sys/class/block/mmcblk0p*/uevent; do
 name=$(sed -n 's/^PARTNAME=//p' "$p")
 case "$name" in system|vendor|cust) echo "$p $name";; esac
done
for p in /android /mnt/android /mnt/system /opt/android; do
 if [ -d "$p" ]; then echo "EXISTING_ANDROID_ROOT=$p"; fi
done
