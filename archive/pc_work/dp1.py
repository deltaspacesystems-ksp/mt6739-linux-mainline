import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /proc/uptime
ps | grep -E 'gpupresent|gpu-present|fbbridge|Xvfb|labwc|waybar' | grep -v grep
ls -l /mnt/root/root/.use_gpu 2>&1; tail -n 3 /tmp/gpup.log 2>&1; tail -n 3 /tmp/gpup.out 2>&1
cat /tmp/session.log 2>&1 | tail -n 3; cat /sys/class/leds/lcd-backlight/brightness
""", timeout=30))
