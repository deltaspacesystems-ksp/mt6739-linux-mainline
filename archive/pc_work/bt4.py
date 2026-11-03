import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root sh -c 'cat /etc/apk/repositories; cat /etc/alpine-release'
""", timeout=40))
