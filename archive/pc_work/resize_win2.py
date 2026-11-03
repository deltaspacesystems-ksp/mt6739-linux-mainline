import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root /bin/sh -c '
export DISPLAY=:0
W=$(xdotool search --onlyvisible --name "" | tail -n 1)
xdotool windowsize $W 736 1440; xdotool windowmove $W 0 0
sleep 3
xdotool getwindowgeometry $W | tr "\n" " "
'
""", timeout=40))
