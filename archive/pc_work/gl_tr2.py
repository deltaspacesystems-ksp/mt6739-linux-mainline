import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
tail -n 40 /tmp/tr.txt
""", timeout=30))
