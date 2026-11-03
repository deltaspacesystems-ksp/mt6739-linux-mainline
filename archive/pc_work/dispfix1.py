import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- fb0 blank state:'; cat /sys/class/graphics/fb0/blank 2>&1
echo '--- backlight (jesli jest):'; for b in /sys/class/leds/lcd-backlight /sys/class/backlight/*; do [ -d "$b" ] && echo "$b: $(cat $b/brightness 2>&1) / max $(cat $b/max_brightness 2>&1)"; done
echo '--- mtkfb debugfs stan:'; cat /sys/kernel/debug/mtkfb 2>&1 | head -n 8
echo '--- wymus unblank + pan:'
/usr/bin/fbsolid lines 2944 2>&1
""", timeout=30))
