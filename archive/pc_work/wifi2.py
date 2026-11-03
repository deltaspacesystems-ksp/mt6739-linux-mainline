import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
conf = 'ctrl_interface=/run/wpa_supplicant\nupdate_config=1\ncountry=PL\nnetwork={\n  ssid="YOUR_SSID"\n  psk="YOUR_WIFI_PASSWORD"\n  key_mgmt=WPA-PSK\n}\n'
y5put.put("/mnt/root/etc/wpa_supplicant/wpa_supplicant.conf", conf, 0o600)
print(y5sh.run(r"""
chroot /mnt/root sh -c 'pkill wpa_supplicant; mkdir -p /run/wpa_supplicant; wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf -D nl80211 2>&1; sleep 8; wpa_cli -i wlan0 status 2>&1 | head -n 12'
""", timeout=60))
