import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg -c >/dev/null
insmod /aroot/vendor/lib/modules/wmt_chrdev_wifi.ko 2>&1; echo rc1=$?
insmod /aroot/vendor/lib/modules/wlan_drv_gen2.ko 2>&1; echo rc2=$?
sleep 2
grep -E 'wmt|wlan' /proc/modules | cut -d' ' -f1,2
dmesg | grep -v -E 'VcoreFS|charger|Power/|DISP|Thermal' | head -n 30
grep -i -E 'wifi|wmt' /proc/devices; ls /sys/class | grep -i -E 'wifi|wmt'
for x in /sys/class/wmtWifi/* /sys/class/*/wmtWifi; do [ -f $x/dev ] && echo "$x $(cat $x/dev)"; done 2>/dev/null
ls /sys/class/net
""", timeout=40))
