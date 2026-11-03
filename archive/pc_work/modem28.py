import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
S="LD_PRELOAD=/data/propshim.so"
pkill ccci_mdinit; pkill ccci_fsd; sleep 1
( chroot /aroot /system/bin/sh -c "$S exec /vendor/bin/ccci_fsd 0" >/tmp/fsd.out 2>&1 </dev/null & ); sleep 1
dmesg -c >/dev/null
( chroot /aroot /system/bin/sh -c "$S exec /vendor/bin/ccci_mdinit 0" >/tmp/mdinit.out 2>&1 </dev/null & ); sleep 45
cat /sys/kernel/ccci/boot
dmesg | grep -i -E 'ccci' | grep -v -E 'rbf\]|crb:|ccci_mdinit\]\[ccci1/(chr|shm|pot)' | head -n 70
""", timeout=80))
