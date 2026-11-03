import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("ls -la /aroot/data/gltest 2>&1; md5sum /aroot/data/gltest 2>&1", timeout=20))
