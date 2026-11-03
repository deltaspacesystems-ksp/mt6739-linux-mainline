import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
echo uptime: $(cat /proc/uptime)
mount | grep pstore || (mount -t pstore pstore /sys/fs/pstore 2>&1; mount | grep pstore)
ls -l /sys/fs/pstore 2>&1
for f in /sys/fs/pstore/*; do echo "=== $f ($(wc -c < $f) B)"; tail -c 1500 $f; done 2>&1 | head -n 60
echo --- reboot reason:
grep -o 'boot_reason=[0-9]*\|reboot_reason=[a-z]*\|bootreason=[a-z_]*' /proc/cmdline
"""
print(y5sh.run(cmd, timeout=40))
