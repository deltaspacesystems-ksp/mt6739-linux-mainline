import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- /proc/devices (majory 150-160):'; grep -E '^(15[0-9]|16[0-9])' /proc/devices
echo '--- /sys/class (wmt/wifi):'; ls /sys/class | grep -i -E 'wmt|wifi|stp'
echo '--- /sys/class/*/dev content jesli jest:'
for c in $(ls /sys/class | grep -i -E 'wmt|wifi|stp'); do echo "-- $c --"; ls /sys/class/$c; done
""", timeout=30))
