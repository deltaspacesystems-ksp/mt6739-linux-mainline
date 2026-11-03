import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- bateria:'; cat /sys/class/power_supply/battery/capacity 2>&1; cat /sys/class/power_supply/battery/status 2>&1
echo '--- uptime:'; cut -d' ' -f1 /proc/uptime
echo '--- procesy problematyczne:'; ps | grep -E 'gltest|wmt_launcher' | grep -v grep
echo '--- moduly wifi nadal zaladowane?:'; lsmod
echo '--- wmt_launcher nadal dziala?:'; ps | grep wmt_launcher
""", timeout=30))
