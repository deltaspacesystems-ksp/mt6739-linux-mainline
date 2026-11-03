import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
code = r'''
import os, select, time
fd = os.open("/dev/stpbt", os.O_RDWR)
def cmd(op, params=b""):
    pkt = bytes([1, op & 0xff, op >> 8, len(params)]) + params
    os.write(fd, pkt)
    t = time.time(); out = b""
    while time.time() - t < 3:
        r, _, _ = select.select([fd], [], [], 0.5)
        if r:
            out += os.read(fd, 512)
            if len(out) >= 7: break
    return out
print("reset ->", cmd(0x0c03).hex())
print("read_local_version ->", cmd(0x1001).hex())
print("read_bd_addr ->", cmd(0x1009).hex())
'''
y5put.put("/mnt/root/tmp/bt.py", code)
print(y5sh.run(r"""
dmesg -c >/dev/null
chroot /mnt/root python3 /tmp/bt.py 2>&1
dmesg | grep -i -E 'BT|STP|WMT' | grep -v -E 'wlan|charger|batt' | head -n 12
""", timeout=40))
