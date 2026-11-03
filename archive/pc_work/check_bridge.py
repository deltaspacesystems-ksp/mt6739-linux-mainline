import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- procesy fbbridge:'; ps | grep -E 'python3|fbbridge' | grep -v grep
echo '--- fbbridge.log:'; cat /mnt/root/tmp/fbbridge.log 2>&1 | tail -n 15
echo '--- /sys/class/graphics/fb0/blank:'; cat /sys/class/graphics/fb0/blank 2>&1
echo '--- backlight:'; cat /sys/class/leds/lcd-backlight/brightness 2>&1
""", timeout=20))
