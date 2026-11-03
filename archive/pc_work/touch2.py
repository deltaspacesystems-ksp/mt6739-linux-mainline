import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
pkill -f 'while true; do chroot /aroot'; pkill gpupresent; pkill -f fbbridge.py; sleep 1
( chroot /mnt/root sh -c 'DISPLAY=:0 python3 /usr/local/bin/fbbridge.py >/tmp/fbbridge.log 2>&1' & )
sleep 4; ps | grep -E 'fbbridge|gpupresent' | grep -v grep; tail -n 2 /mnt/root/tmp/fbbridge.log
""", timeout=40))
