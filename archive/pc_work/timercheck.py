import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
echo up=$(cut -d' ' -f1 /proc/uptime)
dmesg | grep -E 'hwboot_timer|bfm_stop|stop.*timer|boot_stage|bfmr_set' | tail -n 5
"""
print(y5sh.run(cmd, timeout=30))
time.sleep(12)
print(y5sh.run(cmd, timeout=30))
