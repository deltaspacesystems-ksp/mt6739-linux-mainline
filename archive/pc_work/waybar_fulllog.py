import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root /bin/sh -c '
export HOME=/root WAYLAND_DISPLAY=wayland-0 XDG_RUNTIME_DIR=/run/user/0 DBUS_SESSION_BUS_ADDRESS=disabled:
pkill waybar; sleep 1
waybar > /tmp/waybar.log 2>&1 &
WPID=$!
sleep 3
echo pid=$WPID
ps -o pid,stat,cmd -p $WPID 2>&1
cat /tmp/waybar.log
'
""", timeout=30))
