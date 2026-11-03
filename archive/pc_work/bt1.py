import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root sh -c 'which btattach hciattach bluetoothd hciconfig bluetoothctl rfkill 2>&1'
dmesg -c >/dev/null
insmod /aroot/vendor/lib/modules/bt_drv.ko 2>&1; echo bt_rc=$?
insmod /aroot/vendor/lib/modules/gps_drv.ko 2>&1; echo gps_rc=$?
sleep 2
grep -i -E 'stp|bt|gps|fm|wmt' /proc/devices
ls /sys/class | grep -i -E 'stp|bt|gps|fm'
for d in /sys/class/*; do n=$(basename $d); case $n in *bt*|*gps*|*stp*) for x in $d/*; do [ -f $x/dev ] && echo "$x $(cat $x/dev)"; done;; esac; done 2>/dev/null
dmesg | grep -i -E 'bt|gps|stp' | grep -v -E 'VcoreFS|charger|batt' | head -n 12
""", timeout=50))
