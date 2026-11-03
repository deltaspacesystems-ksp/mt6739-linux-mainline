import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
print(y5put.put("/usr/local/bin/mkdevs", r"""#!/bin/sh
mkdir -p /dev/input /dev/pts /dev/shm
for d in /sys/class/input/event* /sys/class/input/mouse* /sys/class/tty/tty /sys/class/tty/tty[0-9] /sys/class/tty/tty[0-9][0-9] /sys/class/tty/ptmx /sys/class/graphics/fb* /sys/class/mem/* /sys/class/misc/* /sys/class/drm/card* /sys/class/vc/vcs*; do
  [ -f "$d/dev" ] || continue
  n=$(basename $d); MM=$(cat $d/dev)
  case $d in */input/*) p=/dev/input/$n;; */drm/*) mkdir -p /dev/dri; p=/dev/dri/$n;; *) p=/dev/$n;; esac
  [ -e $p ] || mknod $p c ${MM%:*} ${MM#*:}
done
[ -e /dev/ptmx ] || mknod /dev/ptmx c 5 2
chmod 666 /dev/null /dev/zero /dev/ptmx /dev/urandom /dev/random /dev/tty 2>/dev/null
echo mkdevs_ok
""", "755"))
f = y5sh.run("cat /usr/local/bin/chroot-up", timeout=15).replace("mdev -s 2>/dev/null\n", "/usr/local/bin/mkdevs >/dev/null\n")
print(y5put.put("/usr/local/bin/chroot-up", f, "755"))
print(y5sh.run("chroot-up; echo; ls /dev/input; ls /dev/tty0 /dev/tty1 /dev/fb0 /dev/ptmx /dev/urandom; cat /sys/class/input/event3/device/name", timeout=40))
