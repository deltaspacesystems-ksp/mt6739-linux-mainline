import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
zcat /proc/config.gz 2>/dev/null | grep -E 'CONFIG_BT|CONFIG_BT_HCIVHCI|CONFIG_BT_HCIUART|CONFIG_RFKILL|CONFIG_UHID|CONFIG_UINPUT' | head -n 20
grep -i -E 'hci|bluetooth|l2cap' /proc/net/protocols /proc/misc 2>/dev/null | head; ls /dev/vhci /dev/uhid /dev/uinput 2>&1
ls /sys/class/bluetooth 2>&1 | head -n 3
""", timeout=40))
