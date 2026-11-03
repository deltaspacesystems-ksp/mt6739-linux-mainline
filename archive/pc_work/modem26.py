import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg | grep -v -E 'rbf\]|crb:|VcoreFS|charger|Power/|fgauge|Batt|JEITA|PPM|USB_BOOST|ESD|wdk|usb_state|gadget|mcdi' | awk -F'[][]' '$2+0>1336 && $2+0<1400' | head -n 70
""", timeout=40))
