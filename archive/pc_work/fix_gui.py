import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
print(y5put.put("/usr/local/bin/chroot-up", """#!/bin/sh
R=/mnt/root
mdev -s 2>/dev/null
[ -e /dev/ptmx ] || mknod /dev/ptmx c 5 2
[ -e /dev/urandom ] || mknod /dev/urandom c 1 9
[ -e /dev/random ] || mknod /dev/random c 1 8
mkdir -p /dev/shm /dev/pts
mountpoint -q $R || mount -t ext4 /dev/part_userdata $R
for m in proc sys dev; do mountpoint -q $R/$m || mount --bind /$m $R/$m; done
mountpoint -q $R/dev/pts || mount -t devpts devpts $R/dev/pts 2>/dev/null
mountpoint -q $R/dev/shm || mount -t tmpfs tmpfs $R/dev/shm 2>/dev/null
mountpoint -q $R/tmp || mount -t tmpfs tmpfs $R/tmp
mountpoint -q $R/run || mount -t tmpfs tmpfs $R/run
echo "nameserver 172.16.42.2" > $R/etc/resolv.conf
echo chroot_up_ok
""", "755"))
print(y5sh.run("chroot-up; echo; ls /dev/input /dev/fb0 /dev/tty0 /dev/tty1 2>&1 | tr '\\n' ' '; echo; cat /sys/class/input/event3/device/name", timeout=40))
