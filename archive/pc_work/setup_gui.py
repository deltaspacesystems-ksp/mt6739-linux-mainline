import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
R = "/mnt/root"
print(y5put.put(R+"/etc/X11/xorg.conf", """Section "ServerFlags"
  Option "AutoAddDevices" "false"
  Option "AutoEnableDevices" "false"
  Option "DontVTSwitch" "true"
  Option "BlankTime" "0"
  Option "StandbyTime" "0"
  Option "SuspendTime" "0"
  Option "OffTime" "0"
EndSection
Section "Device"
  Identifier "fb"
  Driver "fbdev"
  Option "fbdev" "/dev/fb0"
  Option "ShadowFB" "true"
EndSection
Section "Monitor"
  Identifier "mon"
EndSection
Section "Screen"
  Identifier "scr"
  Device "fb"
  Monitor "mon"
  DefaultDepth 24
EndSection
Section "InputDevice"
  Identifier "touch"
  Driver "evdev"
  Option "Device" "/dev/input/event3"
EndSection
Section "ServerLayout"
  Identifier "layout"
  Screen "scr"
  InputDevice "touch" "CorePointer"
EndSection
"""))
print(y5put.put(R+"/root/.xinitrc", """#!/bin/sh
xset s off -dpms
openbox &
sleep 1
svkbd -d &
exec st -f 'DejaVu Sans Mono:size=14'
""", "755"))
print(y5put.put("/usr/local/bin/chroot-up", """#!/bin/sh
R=/mnt/root
mountpoint -q $R || mount -t ext4 /dev/part_userdata $R
for m in proc sys dev; do mountpoint -q $R/$m || mount --bind /$m $R/$m; done
mountpoint -q $R/dev/pts || mount -t devpts devpts $R/dev/pts 2>/dev/null
mountpoint -q $R/tmp || mount -t tmpfs tmpfs $R/tmp
mountpoint -q $R/run || mount -t tmpfs tmpfs $R/run
echo "nameserver 172.16.42.2" > $R/etc/resolv.conf
echo chroot_up_ok
""", "755"))
print(y5put.put("/usr/local/bin/startgui", """#!/bin/sh
chroot-up
chroot /mnt/root /bin/sh -c 'pkill Xorg 2>/dev/null; sleep 1; export HOME=/root; (xinit /root/.xinitrc -- /usr/bin/Xorg :0 -noreset -nolisten tcp -novtswitch -sharevts -config /etc/X11/xorg.conf > /tmp/xorg.out 2>&1 &) ; sleep 6; tail -n 5 /tmp/xorg.out; ls /tmp/.X11-unix 2>&1'
""", "755"))
print(y5sh.run("chroot-up && /usr/local/bin/startgui", timeout=60))
