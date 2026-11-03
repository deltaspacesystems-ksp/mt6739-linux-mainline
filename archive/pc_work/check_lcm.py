import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("cat /sys/kernel/debug/mtkfb 2>&1 | head -n 15", timeout=20))
