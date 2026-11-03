import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
wc -c /proc/last_kmsg; strings /proc/last_kmsg | head -n 30; echo ...; strings /proc/last_kmsg | tail -n 25
""", timeout=30))
