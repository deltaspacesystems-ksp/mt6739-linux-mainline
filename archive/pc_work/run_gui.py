import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("/usr/local/bin/chroot-up; /usr/local/bin/startgui", timeout=60))
time.sleep(4)
print(y5sh.run(r"""
echo '--- procesy:'; ps | grep -E 'Xorg|xinit|openbox|svkbd|[ /]st( |$)' | grep -v grep
echo '--- xorg.out:'; tail -n 12 /mnt/root/tmp/xorg.out 2>&1
echo '--- Xorg.0.log (EE):'; grep -E '\(EE\)' /mnt/root/var/log/Xorg.0.log 2>/dev/null | head -n 10
""", timeout=40))
