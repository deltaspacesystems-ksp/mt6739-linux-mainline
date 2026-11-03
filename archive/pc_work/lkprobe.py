import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("ls /proc | grep -i -E 'lk|bootprof|last|aee|mtprof|mrdump'; ls /sys/fs/pstore; echo ---; cat /proc/bootprof 2>/dev/null | head -n 40", timeout=30))
