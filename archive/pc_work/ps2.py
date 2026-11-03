import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -l /sys/fs/pstore; mount | grep pstore
dmesg | grep -i -E 'pstore|ramoops|ram_console|last_kmsg|persistent' | head -n 12
ls /proc | grep -i -E 'last_kmsg|ram'
cat /proc/cmdline | tr ' ' '\n' | grep -i -E 'reason|wdt|bootup'
""", timeout=30))
