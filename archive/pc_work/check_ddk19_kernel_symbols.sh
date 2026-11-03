#!/bin/bash
set -eu
export LC_ALL=C
baseline=/mnt/d/y5_linux_work/out/kernel-v59-defined-symbols.txt
if [ ! -s "$baseline" ]; then
 aarch64-linux-gnu-nm -g --defined-only /home/y5/linux-7.3/vmlinux |
   awk '{ print $3 }' | sort -u > /tmp/mfw-kernel-defined.txt
 if grep -qx 'PVRSRVDriverInit' /tmp/mfw-kernel-defined.txt; then
  echo 'Need preserved pre-DDK v59 symbol baseline, not a kernel containing DDK' >&2
  exit 1
 fi
 cp /tmp/mfw-kernel-defined.txt "$baseline"
fi
awk '{ print $2 }' /mnt/d/y5_linux_work/out/ddk19-undefined.txt |
  sort -u > /tmp/mfw-ddk-undefined.txt
comm -23 /tmp/mfw-ddk-undefined.txt "$baseline" > /mnt/d/y5_linux_work/out/ddk19-missing-kernel-symbols.txt
cat /mnt/d/y5_linux_work/out/ddk19-missing-kernel-symbols.txt
test ! -s /mnt/d/y5_linux_work/out/ddk19-missing-kernel-symbols.txt
