import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
echo '--- kallsyms bfm/bfr:'; grep -i -E ' [tT] .*(bfm|bfr)' /proc/kallsyms | head -n 25
echo '--- proc/sys/sysfs nodes:'; ls /proc | grep -i -E 'bf|boot'; ls /sys/class | grep -i -E 'bf|boot|hw'; find /sys -maxdepth 3 -iname '*bfm*' -o -maxdepth 3 -iname '*bfr*' 2>/dev/null | head -n 20
echo '--- pstore bfm lines:'; grep -i -E 'bfm|bfr|boot.?fail|boot_success|bootcomplete|timeout' /sys/fs/pstore/console-ramoops | head -n 30
"""
print(y5sh.run(cmd, timeout=60))
