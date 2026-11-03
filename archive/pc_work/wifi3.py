import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root sh -c 'udhcpc -i wlan0 -n -q -t 5 2>&1 | tail -n 5; ip -4 addr show wlan0 | grep inet; ip route | head -n 5'
chroot /mnt/root sh -c 'ping -c 3 -W 3 8.8.8.8 2>&1 | tail -n 3; nslookup example.com 2>&1 | tail -n 3'
""", timeout=80))
