import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("/usr/bin/fbsolid solid 00 ff 00 ff", timeout=20))
