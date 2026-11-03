import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
code = r'''
import fcntl, os, sys, time
fd = os.open("/dev/ccci_monitor", os.O_RDWR)
def io(cmd, arg=0):
    try:
        r = fcntl.ioctl(fd, cmd, arg); print(hex(cmd), "->", r if not isinstance(r, bytes) else r.hex())
    except OSError as e:
        print(hex(cmd), "ERR", e)
io(0x430c, bytearray(16))
time.sleep(3)
print(open("/sys/kernel/ccci/boot").read().split("|")[0])
'''
y5put.put("/mnt/root/tmp/ci2.py", code)
print(y5sh.run(r"""
dmesg -c >/dev/null
chroot /mnt/root python3 /tmp/ci2.py 2>&1
dmesg | grep -i -E 'ccci' | grep -v -E 'rbf\]|crb:|monitor' | head -n 20
""", timeout=40))
