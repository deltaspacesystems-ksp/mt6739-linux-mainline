import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root /bin/sh -c '
export HOME=/root WAYLAND_DISPLAY=wayland-0 XDG_RUNTIME_DIR=/run/user/0
pkill waybar; sleep 1
waybar > /tmp/waybar.log 2>&1 &
sleep 2
ps | grep waybar | grep -v grep
cat /tmp/waybar.log | tail -n 10
'
""", timeout=30))
