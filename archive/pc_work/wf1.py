import sys, datetime
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
now = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
print(y5sh.run(r"""
cat /proc/uptime; mountpoint -q /aroot/vendor && echo aroot_ok
date -u -s "%s" >/dev/null
( /usr/local/bin/wcn-up >/tmp/wcn-up.log 2>&1 & )
sleep 45
cat /tmp/wcn-up.log
ip -4 addr show wlan0 2>&1 | grep inet; ip route | head -n 3
chroot /mnt/root sh -c 'wpa_cli -i wlan0 status 2>&1 | grep -E "ssid|wpa_state"; ping -c 2 -W 3 8.8.8.8 2>&1 | tail -n 2'
""" % now, timeout=100))
