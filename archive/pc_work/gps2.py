import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls /sys/class/misc | grep -i gps; for m in /sys/class/misc/*gps*; do echo "$m $(cat $m/dev)"; done
mkdir -p /aroot/data/misc/gps /aroot/data/agps_supl
for m in /sys/class/misc/*gps*; do n=$(basename $m); MM=$(cat $m/dev); [ -e /dev/$n ] || mknod /dev/$n c ${MM%:*} ${MM#*:}; chmod 666 /dev/$n; done
ls -l /dev | grep -i gps
strings -a /aroot/vendor/bin/mnld | grep -i -E 'nmea|ttyg|socket|mnl_|port' | head -n 20
""", timeout=40))
