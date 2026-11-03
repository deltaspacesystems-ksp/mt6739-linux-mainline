import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("cat /tmp/ready 2>&1; chroot /mnt/root sh -c 'ps' 2>&1 | grep -E 'Xvfb|labwc|waybar|python3' ; cat /sys/class/leds/lcd-backlight/brightness 2>&1", timeout=20))
