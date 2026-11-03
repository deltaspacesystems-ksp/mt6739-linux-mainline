import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /proc/device-tree
ls | grep -i -E 'msdc|mmc|usb|mt_usb|otg|usb20|usb_phy|pwrap|pmic|topckgen|infracfg|apmixed|scpsys|gpio|pinctrl|eint|i2c|spi|kpd|keypad|rtc|auxadc|wdt|watchdog|uart|sleep|mcucfg|sysirq' | head -n 60
""", timeout=30))
