import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mount | grep '/mnt/root/ro'
ls /mnt/root/ro 2>&1
ls /mnt/root/ro/vendor 2>&1 | head -n 3
chroot /mnt/root ls /ro/vendor/lib/modules 2>&1 | head -n 3
""", timeout=30))
