import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root sh -c 'which wpa_supplicant iw wpa_cli udhcpc dhclient 2>&1'
ifconfig wlan0 up 2>&1; ip link show wlan0 2>&1 | head -n 3
chroot /mnt/root sh -c 'iw dev wlan0 scan 2>&1 | grep -E "SSID|signal|freq" | head -n 40'
""", timeout=60))
