import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /proc/uptime
for f in capacity status voltage_now current_now temp; do echo "$f: $(cat /sys/class/power_supply/battery/$f 2>&1)"; done
cat /sys/class/power_supply/*/online 2>/dev/null | tr '\n' ' '; echo
ps | grep -E 'wcn-up|wmt_' | grep -v grep; grep -E '^(wmt|wlan)' /proc/modules | cut -d' ' -f1
""", timeout=30))
