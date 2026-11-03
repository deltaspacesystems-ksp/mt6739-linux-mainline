import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
echo '--- /sys/class/misc/hw_bfm:'; ls -l /sys/class/misc/hw_bfm/; cat /sys/class/misc/hw_bfm/dev
echo '--- hwboot symbols:'; grep -i -E ' [tT] (hwboot|boot_fail|bootfail|bfmr_.*(ioctl|notify|boot_ok|success|stop|disable)|.*boot_success)' /proc/kallsyms | head -n 25
echo '--- boot_mode / reboot_watchdog:'; cat /proc/boot_mode 2>&1 | head -n 3; ls -l /proc/reboot_watchdog; cat /proc/reboot_watchdog 2>&1 | head -n 3
echo '--- dmesg hwboot:'; dmesg | grep -i -E 'hwboot|bfm_|bfmr' | head -n 20
echo uptime: $(cut -d' ' -f1 /proc/uptime)
"""
print(y5sh.run(cmd, timeout=60))
