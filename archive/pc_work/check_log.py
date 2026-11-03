import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(repr(y5sh.run("cat /tmp/gltest.log 2>&1; echo END_MARKER", timeout=15)))
