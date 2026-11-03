import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- /tmp/ready:'; cat /tmp/ready 2>&1
echo '--- procesy w chroot:'; chroot /mnt/root sh -c 'ps' 2>&1 | grep -E 'Xvfb|labwc|waybar|wvkbd|python3|sshd' 
echo '--- bateria:'; cat /sys/class/power_supply/battery/capacity 2>&1; cat /sys/class/power_supply/battery/status 2>&1
""", timeout=30))
