import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mount --bind /aroot/vendor/lib64/hw/gralloc.mt6739.so /aroot/vendor/lib64/hw/gralloc.default.so 2>&1
( chroot /mnt/root python3 /tmp/lg.py >/tmp/lg.err 2>&1 & ); sleep 2
timeout 15 chroot /aroot /data/gltest2 2>&1 </dev/null | tail -n 12; sleep 1
strings /mnt/root/tmp/logdw.txt | tail -n 20
""", timeout=60))
