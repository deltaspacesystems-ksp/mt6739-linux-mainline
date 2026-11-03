import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
[ -e /dev/wmtWifi ] || mknod /dev/wmtWifi c 153 0
timeout 10 sh -c "python3 -c \"open('/dev/wmtWifi','wb',buffering=0).write(b'1')\"" 2>&1 || chroot /mnt/root python3 -c "open('/dev/wmtWifi','wb',buffering=0).write(b'1')" 2>&1
echo wrote_1
""", timeout=20))
