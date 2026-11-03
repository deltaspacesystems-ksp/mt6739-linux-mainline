import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /proc/uptime
pkill -f wcn-up; pkill wmt_loader; pkill wmt_launcher; sleep 1
ps | grep -E 'wcn-up|wmt_' | grep -v grep
grep -E '^(wmt|wlan)' /proc/modules | cut -d' ' -f1
cat /proc/uptime
""", timeout=30))
