import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for p in /proc/[0-9]*; do ls -l $p/fd 2>/dev/null | grep -q ccci_monitor && echo "holder: ${p#/proc/} $(cat $p/comm)"; done
""", timeout=40))
