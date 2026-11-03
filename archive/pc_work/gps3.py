import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mkdir -p /aroot/dev/socket /aroot/data/misc/gps /aroot/data/agps_supl; dmesg -c >/dev/null
( chroot /aroot /system/bin/sh -c 'LD_PRELOAD=/data/propshim.so exec /vendor/bin/mnld' >/tmp/mnld.out 2>&1 </dev/null & )
sleep 12
ps | grep mnld | grep -v grep; tail -n 6 /tmp/mnld.out
ls -l /aroot/dev/socket /aroot/data/misc/gps /aroot/data/agps_supl 2>&1 | head -n 20
dmesg | grep -i -E 'gps|stp|wmt|mnl' | grep -v -E 'wlan|charger' | head -n 20
""", timeout=50))
