import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- procesy pelne:'; chroot /mnt/root sh -c 'ps' | grep -E 'Xvfb|labwc|waybar|foot|wvkbd'
echo '--- xvfb.log:'; tail -n 10 /mnt/root/tmp/xvfb.log 2>&1
echo '--- labwc.log:'; tail -n 15 /mnt/root/tmp/labwc.log 2>&1
echo '--- okna X:'; chroot /mnt/root /bin/sh -c 'export DISPLAY=:0; xdotool search --onlyvisible --name "" 2>&1'
""", timeout=30))
