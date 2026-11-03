import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
/usr/local/bin/chroot-up >/dev/null
chroot /mnt/root /bin/sh -c '
pkill weston; pkill cage; pkill foot; sleep 1
export HOME=/root XDG_RUNTIME_DIR=/run/user/0 DISPLAY=:0 WLR_BACKENDS=x11 WLR_RENDERER=pixman WLR_NO_HARDWARE_CURSORS=1
(cage -d -- foot > /tmp/cage.log 2>&1 &)
sleep 6
echo "--- procesy:"; ps | grep -E "cage|foot|Xvfb" | grep -v grep
echo "--- cage.log:"; tail -n 14 /tmp/cage.log
'
""", timeout=60))
