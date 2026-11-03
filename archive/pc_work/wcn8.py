import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
[ -e /dev/wmtWifi ] || mknod /dev/wmtWifi c 153 0; chmod 666 /dev/wmtWifi
dmesg -c >/dev/null
( echo 1 > /dev/wmtWifi; echo write_rc=$? > /tmp/wifi_on.rc ) &
sleep 20
cat /tmp/wifi_on.rc 2>&1
dmesg | grep -E 'WMT|STP|HIF|PATCH|CONSYS|wlan|WIFI|MTK-WIFI|firmware|ROM' | head -n 60
ls /sys/class/net | grep -E 'wlan|p2p|ap'
""", timeout=70))
