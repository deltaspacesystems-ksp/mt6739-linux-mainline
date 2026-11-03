import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls /usr/local/bin; grep -rl aroot /usr/local/bin /init /mnt/root/usr/local/bin 2>/dev/null
""", timeout=30))
