import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- backlight:'; cat /sys/class/leds/lcd-backlight/brightness 2>&1
echo '--- ready:'; cat /tmp/ready 2>&1
echo '--- mountpoint:'; mountpoint /mnt/root 2>&1
echo '--- procesy chroot:'; chroot /mnt/root sh -c 'ps' 2>&1 | grep -E 'Xvfb|labwc|python3|waybar'
echo '--- uptime:'; cut -d' ' -f1 /proc/uptime
""", timeout=20))
