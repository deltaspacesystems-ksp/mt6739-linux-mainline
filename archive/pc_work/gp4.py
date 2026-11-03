import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push, y5put
src = open(r"D:\y5_linux_work\guisrc\fbbridge.py", encoding="utf-8").read()
y5put.put("/mnt/root/usr/local/bin/fbbridge.py", src, 0o755)
print(push.push(r"D:\y5_linux_work\gpupresent", "/aroot/data/gpupresent", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/gpupresent
pkill -f fbbridge.py; sleep 1
( chroot /mnt/root sh -c 'DISPLAY=:0 GPUPRESENT=1 python3 /usr/local/bin/fbbridge.py >/tmp/fbbridge.log 2>&1' & )
mkdir -p /aroot/xfb; mountpoint -q /aroot/xfb || mount --bind /mnt/root/tmp/xfb /aroot/xfb
( while true; do chroot /aroot /data/gpupresent >/tmp/gpup.out 2>&1; sleep 2; done & )
sleep 6; tail -n 3 /tmp/gpup.out; ps | grep -E 'fbbridge|gpupresent' | grep -v grep
""", timeout=60))
