import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for p in 10389 10392; do grep -E 'State|PPid|Threads' /proc/$p/status | tr '\n' ' '; echo; cat /proc/$p/wchan; echo; done
top -b -n1 | head -n 12
""", timeout=30))
