import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
grep -i -E 'wmt|stp|gps|bt|fm|wifi' /proc/devices
ls /sys/class | grep -i -E 'wmt|stp|gps|bt|fm|wifi|conn'
for d in /sys/class/wmt /sys/class/stpwmt /sys/class/*stp* /sys/class/*wmt* /sys/class/*gps* /sys/class/*bt*; do [ -d $d ] && for x in $d/*; do [ -f $x/dev ] && echo "$x $(cat $x/dev)"; done; done 2>/dev/null
ls /proc/driver 2>/dev/null
""", timeout=40))
