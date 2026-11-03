import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
uname -r; cat /proc/uptime; cat /proc/cmdline | tr ' ' '\n' | grep -i -E 'bootreason|boot_reason|reboot_reason'
ls -l /tmp/prev 2>&1 | head
for f in /tmp/prev/console-ramoops* ; do [ -f $f ] && { echo "== $f"; tail -n 25 $f; }; done 2>/dev/null
mountpoint /vendor; ps | grep -E 'gpupresent|fbbridge' | grep -v grep | head -n 3
""", timeout=30))
