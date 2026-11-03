(
set -eu
if ! grep -q 'microfw POSIX shm' /etc/y5/rc.sh; then
 cp /etc/y5/rc.sh /etc/y5/rc.sh.before-shm
 cat >> /etc/y5/rc.sh <<'END'

# microfw POSIX shm: required by Wayland/wlroots keymaps and clients.
mkdir -p /dev/shm
mountpoint -q /dev/shm || mount -t tmpfs -o mode=1777,nosuid,nodev tmpfs /dev/shm
END
fi
sh -n /etc/y5/rc.sh
)
