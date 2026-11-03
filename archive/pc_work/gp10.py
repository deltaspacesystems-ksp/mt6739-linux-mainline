import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
y5put.put("/tmp/gpl.sh", "#!/bin/sh\nwhile true; do chroot /aroot /data/gpupresent >/tmp/gpup.out 2>&1; sleep 2; done\n", 0o755)
print(y5sh.run(r"""
for p in $(pidof gpupresent); do pp=$(awk '{print $4}' /proc/$p/stat); kill -9 $pp 2>/dev/null; done
pkill -9 gpupresent; sleep 1
echo after_kill: $(pidof gpupresent)
( sh /tmp/gpl.sh >/dev/null 2>&1 & )
sleep 5; ps | grep -E 'gpupresent|gpl.sh' | grep -v grep; tail -n 2 /tmp/gpup.out
""", timeout=40))
