import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root /bin/sh -c '
export DISPLAY=:0
xdotool search --onlyvisible --name "" 2>/dev/null | head -n 5
for w in $(xdotool search --onlyvisible --name "" 2>/dev/null); do xdotool getwindowname $w; xdotool getwindowgeometry $w | tr "\n" " "; echo; done
'
""", timeout=40))
