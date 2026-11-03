import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
find /sys/devices/platform/mt_usb /sys/devices/platform/musb-hdrc /sys/devices/platform/otg_iddig -maxdepth 3 -type f \( -name 'mode' -o -name 'cmode' -o -name 'vbus' -o -name 'power_role' -o -name 'iddig*' -o -name 'otg*' -o -name 'saving' -o -name 'portmode' \) 2>/dev/null
ls /sys/class/udc; cat /sys/class/udc/*/state 2>/dev/null
ls /sys/kernel/debug/musb* /sys/kernel/debug/usb 2>&1 | head
grep -i -E 'otg|host' /proc/config.gz 2>/dev/null | head -n 2; zcat /proc/config.gz 2>/dev/null | grep -E 'USB_MUSB|USB_HOST|USB_STORAGE|USB_HID|USB_ACM|USB_SERIAL|MTK_USB|OTG|USB_PRINTER|USB_NET' | head -n 20
""", timeout=40))
