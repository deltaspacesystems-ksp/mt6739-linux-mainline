#!/bin/bash
set -eu
test -s /tmp/mfw-kernel-defined.txt
if grep -qx 'PVRSRVDriverInit' /tmp/mfw-kernel-defined.txt; then
 echo 'Refusing a symbol baseline that already contains the native DDK' >&2
 exit 1
fi
target=/mnt/d/y5_linux_work/out/kernel-v59-defined-symbols.txt
if [ -e "$target" ]; then cmp /tmp/mfw-kernel-defined.txt "$target"; else cp /tmp/mfw-kernel-defined.txt "$target"; fi
sha256sum "$target"
