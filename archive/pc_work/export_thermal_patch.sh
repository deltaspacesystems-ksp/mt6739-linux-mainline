#!/bin/bash
set -eu
baseline=/home/y5/pmic-pristine
mkdir -p "$baseline"
files='drivers/video/Makefile drivers/video/y5disp.c drivers/gpu/drm/sysfb/drm_sysfb_modeset.c init/main.c drivers/usb/musb/mediatek.c drivers/base/dd.c kernel/panic.c arch/arm64/kernel/pi/map_range.c arch/arm64/kernel/head.S arch/arm64/kernel/setup.c include/linux/y5cp.h drivers/power/supply/Makefile drivers/power/supply/Kconfig drivers/power/supply/y5-pmic-monitor.c drivers/power/supply/y5-ntc.h'
original=''
for file in $files; do
 case "$file" in drivers/video/y5disp.c|include/linux/y5cp.h|drivers/power/supply/y5-pmic-monitor.c|drivers/power/supply/y5-ntc.h) ;; *) original="$original linux-7.3/$file";; esac
done
tar -xf /home/y5/linux-7.3.tar.xz -C "$baseline" $original
cd /home/y5/linux-7.3
out=/mnt/d/y5_linux_work/export/y5-kernel-7.3-v50.patch
: > "$out"
for file in $files; do
 if [ -f "$baseline/linux-7.3/$file" ]; then
  diff -u --label "a/$file" --label "b/$file" "$baseline/linux-7.3/$file" "$file" >> "$out" && rc=0 || rc=$?
 else
  diff -u --label /dev/null --label "b/$file" /dev/null "$file" >> "$out" && rc=0 || rc=$?
 fi
 test "$rc" -le 1
done
cd "$baseline/linux-7.3"
patch --dry-run -p1 < "$out" > /mnt/d/y5_linux_work/out/v50-patch-dryrun.log
wc -l "$out"
grep -c '^+++ ' "$out"
