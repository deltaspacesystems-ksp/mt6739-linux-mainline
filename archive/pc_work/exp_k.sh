set -e
OUT=/mnt/d/y5_linux_work/export
mkdir -p $OUT /tmp/pr
cd /tmp/pr
F="init/main.c drivers/usb/musb/mediatek.c drivers/base/dd.c kernel/panic.c arch/arm64/kernel/pi/map_range.c arch/arm64/kernel/head.S arch/arm64/kernel/setup.c include/linux/y5cp.h"
tar -xf /home/y5/linux-7.3.tar.xz $(for f in $F; do echo linux-7.3/$f; done) 2>/dev/null || true
cd /home/y5/linux-7.3
: > $OUT/y5-kernel-7.3.patch
for f in $F; do
  if [ -f /tmp/pr/linux-7.3/$f ]; then diff -u --label a/$f --label b/$f /tmp/pr/linux-7.3/$f $f >> $OUT/y5-kernel-7.3.patch || true
  else diff -u --label /dev/null --label b/$f /dev/null $f >> $OUT/y5-kernel-7.3.patch || true; fi
done
cp .config $OUT/config-7.3-y5
cp /mnt/d/y5_linux_work/bld.sh $OUT/bld.sh
wc -l $OUT/y5-kernel-7.3.patch; grep -c '^+++ ' $OUT/y5-kernel-7.3.patch; ls -l $OUT
