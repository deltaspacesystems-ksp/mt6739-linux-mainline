import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
pkill -9 ccci_mdinit; sleep 1; dmesg -c >/dev/null
( chroot /aroot /system/bin/sh -c "LD_PRELOAD=/data/propshim.so exec /vendor/bin/ccci_mdinit 0" >/tmp/mdinit.out 2>&1 </dev/null & )
sleep 50
cat /sys/kernel/ccci/boot | cut -c1-6
dmesg | grep -i -E 'ccci|md_' | grep -v -E 'rbf\]|crb:|ccci1/(chr|shm|pot)|ccci_mdinit\]\[' | head -n 40
""", timeout=90))
