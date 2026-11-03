import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("cat /mnt/root/tmp/waybar.log 2>&1 | tail -n 30", timeout=20))
