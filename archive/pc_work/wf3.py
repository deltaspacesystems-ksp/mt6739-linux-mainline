import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
try:
    print(y5sh.run(r"""
cat /proc/uptime
cat /tmp/wcn-up.log
ip -4 addr show wlan0 2>&1 | grep inet
ps | grep -E 'waybar|gpupresent|wpa_supplicant' | grep -v grep | wc -l
""", timeout=20))
except Exception as e:
    print("BRAK POLACZENIA:", e)
