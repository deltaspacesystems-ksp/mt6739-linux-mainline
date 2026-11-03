import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
print(y5put.put("/usr/local/bin/startgui", """#!/bin/sh
/usr/local/bin/chroot-up >/dev/null
chroot /mnt/root /bin/sh -c 'pkill Xorg; pkill xinit; sleep 1; export HOME=/root; mkdir -p /tmp/.X11-unix /var/log; (xinit /root/.xinitrc -- /usr/libexec/Xorg :0 -noreset -nolisten tcp -novtswitch -sharevts -config /etc/X11/xorg.conf -logfile /tmp/Xorg.0.log > /tmp/xorg.out 2>&1 &); sleep 8; tail -n 6 /tmp/xorg.out; ls /tmp/.X11-unix'
""", "755"))
print(y5sh.run("/usr/local/bin/startgui", timeout=60))
print(y5sh.run(r"""
echo '--- procesy:'; ps | grep -E 'Xorg|openbox|svkbd|/usr/bin/st' | grep -v grep
echo '--- Xorg.0.log (EE/WW):'; grep -E '\(EE\)|\(WW\)' /mnt/root/tmp/Xorg.0.log 2>/dev/null | head -n 14
echo '--- koniec logu:'; tail -n 6 /mnt/root/tmp/Xorg.0.log 2>/dev/null
""", timeout=40))
