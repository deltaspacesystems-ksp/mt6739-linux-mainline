import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for d in /sys/class/*/; do :; done
ls /sys/class | grep -i -E 'wmt|detect|stp|gps|bt' ; ls /sys/class/misc | grep -i -E 'wmt|detect'
head -c 20 /aroot/vendor/bin/wmt_loader | od -An -tx1 | head -n 2
strings -a /aroot/vendor/bin/wmt_loader | grep -E '^/|\.ko|insmod|chip|mknod' | head -n 20
""", timeout=40))
