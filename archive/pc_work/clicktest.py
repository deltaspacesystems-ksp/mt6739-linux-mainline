import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root /bin/sh -c '
export DISPLAY=:0
xdotool mousemove 74 31 click 1; sleep 3
xdotool mousemove 623 31 click 1; sleep 2
ps | grep -E "foot|wvkbd" | grep -v grep
'
""", timeout=40))
