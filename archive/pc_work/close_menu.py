import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root /bin/sh -c '
export DISPLAY=:0
xdotool mousemove 100 1200 click 1
pkill waybar
sleep 1
waybar > /tmp/waybar.log 2>&1 &
sleep 1
ps | grep -E "waybar" | grep -v grep
'
""", timeout=30))
