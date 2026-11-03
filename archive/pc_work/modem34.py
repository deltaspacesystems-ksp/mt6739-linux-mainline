import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -l /mnt/root/tmp/ci.py /dev/ccci_monitor 2>&1
chroot /mnt/root python3 /tmp/ci.py 0x4300 2>&1; echo rc=$?
""", timeout=40))
