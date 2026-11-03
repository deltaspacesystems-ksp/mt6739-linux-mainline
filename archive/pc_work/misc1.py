import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- leds'; for l in /sys/class/leds/*; do echo "$(basename $l): $(cat $l/brightness 2>/dev/null) max=$(cat $l/max_brightness 2>/dev/null) trig=$(cat $l/trigger 2>/dev/null | tr '\n' ' ' | cut -c1-80)"; done
echo '--- otg/musb'; ls /sys/class/udc 2>&1; ls /sys/devices/platform | grep -i -E 'usb|musb|mt_usb|otg'
for f in /sys/devices/platform/*usb*/mode /sys/devices/platform/mt_usb/*; do [ -f $f ] && echo "$f: $(cat $f 2>/dev/null | head -n 2 | tr '\n' ' ')"; done 2>/dev/null | head -n 20
ls /sys/class/usb_role /sys/class/extcon 2>&1 | head; cat /proc/mtk_usb/* 2>/dev/null | head -n 5
echo '--- gps'; ls /dev | grep -i -E 'gps|stp|wmt|mtk_|fm'; ls /sys/class/misc | grep -i -E 'gps|wmt|stp|fm'
echo '--- wifi/bt'; ls /sys/class/net | grep -i -E 'wlan|p2p|ap0'; ls /sys/class/rfkill 2>&1 | head -n 3
""", timeout=40))
