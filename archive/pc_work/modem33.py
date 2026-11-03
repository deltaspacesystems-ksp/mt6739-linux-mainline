import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
code = r'''
import fcntl, os, sys
fd = os.open("/dev/ccci_monitor", os.O_RDWR)
for cmd in [int(x,0) for x in sys.argv[1:]]:
    try:
        r = fcntl.ioctl(fd, cmd, 0)
        print(hex(cmd), "->", r)
    except OSError as e:
        print(hex(cmd), "ERR", e)
'''
y5put.put("/mnt/root/tmp/ci.py", code)
print(y5sh.run(r"""
dmesg -c >/dev/null
chroot /mnt/root python3 /tmp/ci.py 0x4300
sleep 3
dmesg | grep -i -E 'ccci' | grep -v -E 'rbf\]|crb:' | tail -n 15
cat /sys/kernel/ccci/boot
""", timeout=40))
