import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("ls -la /mnt/root/run/user/0/ 2>&1", timeout=20))
