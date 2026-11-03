import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg | grep -o 'vbat:[0-9]*' | tail -n 2
dmesg | grep -E 'avgVbat' | tail -n 1
cat /proc/uptime
""", timeout=15))
