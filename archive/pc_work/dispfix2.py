import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo 200 > /sys/class/leds/lcd-backlight/brightness
cat /sys/class/leds/lcd-backlight/brightness
""", timeout=20))
