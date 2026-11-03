import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
pkill -9 mstrace; kill -9 6884 2>&1; sleep 1
dmesg -c >/dev/null
chroot /mnt/root python3 /tmp/ci.py 0x4300 2>&1
sleep 3
dmesg | grep -i -E 'ccci' | grep -v -E 'rbf\]|crb:' | tail -n 15
cat /sys/kernel/ccci/boot
""", timeout=40))
