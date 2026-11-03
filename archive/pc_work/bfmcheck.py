import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
r = lambda c: y5sh.run(c, timeout=30)
print(r("echo up=$(cut -d' ' -f1 /proc/uptime); dmesg | grep -E 'hwboot_timer|bfm_' | tail -n 4"))
time.sleep(12)
print(r("echo up=$(cut -d' ' -f1 /proc/uptime); dmesg | grep -E 'hwboot_timer|bfm_' | tail -n 3"))
