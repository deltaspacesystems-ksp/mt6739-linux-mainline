import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("ps | grep -E 'gltest|chroot'; echo ---; dmesg | tail -n 25", timeout=20))
