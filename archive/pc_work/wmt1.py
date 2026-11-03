import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls /sys/class | grep -i -E 'wmt|stp|gps|bt|wifi|fm|conn'
for d in /sys/class/*wmt* /sys/class/*stp* /sys/class/*gps* /sys/class/*fm*; do [ -d $d ] && for x in $d/*; do [ -f $x/dev ] && echo "$x $(cat $x/dev)"; done; done 2>/dev/null
ls -l /dev | grep -i -E 'wmt|stp|gps|fm|btif|conn'
echo --- init rc; grep -l -i -E 'wmt|connsys|conninfra' /aroot/vendor/etc/init/*.rc /aroot/vendor/etc/init/hw/*.rc 2>/dev/null
echo --- dmesg wmt; dmesg | grep -i -E 'wmt|stp|conn|mt6627|combo|consys' | head -n 20
""", timeout=40))
