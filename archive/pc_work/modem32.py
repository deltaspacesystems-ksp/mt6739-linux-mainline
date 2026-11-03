import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg | grep -i -E 'ccci' | grep -v -E 'rbf\]|crb:|ccci1/(chr|shm)|ccb' | tail -n 30
""", timeout=40))
