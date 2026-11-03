import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot-up >/dev/null
echo '--- procesy:'; ps | grep -E 'Xorg|xinit|openbox|svkbd|/st|weston|cage' | grep -v grep | head
echo '--- xorg.out (chroot /tmp):'; tail -n 15 /mnt/root/tmp/xorg.out 2>&1
echo '--- /dev/input:'; ls /dev/input 2>&1 | tr '\n' ' '
echo; echo '--- Xorg log errors:'; grep -E '\(EE\)|\(WW\).*(fbdev|evdev)' /mnt/root/var/log/Xorg.0.log 2>/dev/null | head -n 12
""", timeout=40))
