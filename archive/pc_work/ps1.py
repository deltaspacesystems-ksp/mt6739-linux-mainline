import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
uname -r; cat /proc/uptime
ls -l /tmp/prev
echo ======== console-ramoops
for f in /tmp/prev/console-ramoops*; do echo "== $f ($(wc -c < $f) B)"; strings $f | head -n 60; done
""", timeout=40))
