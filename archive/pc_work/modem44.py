import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg | grep -E 'ccci1/bfm|ccci1/fsm.*(Dump|queue)' | head -n 3
dmesg | awk '/Dump queue 0/,/ee boot failed/' | grep -v rbf | head -n 40
echo ---- ccci_log tail; tail -c 3000 /proc/ccci_log 2>/dev/null | strings | tail -n 25
""", timeout=40))
