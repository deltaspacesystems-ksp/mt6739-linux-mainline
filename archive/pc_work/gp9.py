import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mv /aroot/data/gpupresent.new /aroot/data/gpupresent
pkill -f fbbridge.py; sleep 1
( chroot /mnt/root sh -c 'DISPLAY=:0 GPUPRESENT=1 python3 /usr/local/bin/fbbridge.py >/tmp/fbbridge.log 2>&1' & )
( while true; do chroot /aroot /data/gpupresent >/tmp/gpup.out 2>&1; sleep 2; done & )
sleep 6; tail -n 2 /tmp/gpup.out; ps | grep -E 'fbbridge|gpupresent' | grep -v grep
""", timeout=40))
