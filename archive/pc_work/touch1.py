import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ps | grep -E 'fbbridge|gpupresent|Xvfb|labwc' | grep -v grep
tail -n 5 /mnt/root/tmp/fbbridge.log; tail -n 3 /tmp/gpup.out
cat /proc/uptime
cat /proc/bus/input/devices | grep -A4 -i 'ts_kit' | head -n 8
""", timeout=30))
