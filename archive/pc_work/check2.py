import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("cat /sys/class/leds/lcd-backlight/brightness 2>&1; chroot /mnt/root sh -c 'ps' | grep -E 'Xvfb|labwc|python3|weston' ; cat /sys/class/power_supply/battery/status 2>&1", timeout=20))
