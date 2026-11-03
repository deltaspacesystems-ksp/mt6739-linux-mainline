import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- sysfs wmtWifi pelna zawartosc:'; find /sys/class/wmtWifi -type f -o -type l 2>&1
echo '--- sysfs wmtdetect pelna zawartosc:'; find /sys/class/wmtdetect -type f -o -type l 2>&1
echo '--- probuj otworzyc /dev/wmtWifi z poziomu python i napisac WiFi ON (symbol WMT_WIFI_ON -> litera moze):'
chroot /mnt/root python3 -c "
f=open('/dev/wmtWifi','r+b', buffering=0)
print('otwarto /dev/wmtWifi ok')
"
""", timeout=40))
