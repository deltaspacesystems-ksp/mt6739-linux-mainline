import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ps | grep -E 'gpupresent|fbbridge' | grep -v grep
pkill gpupresent; pkill gpupresent_bench; sleep 1
echo after; ps | grep -E 'gpupresent|fbbridge' | grep -v grep
""", timeout=30))
