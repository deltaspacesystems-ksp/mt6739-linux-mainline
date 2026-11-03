import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\shim\propshim.so", "/aroot/data/propshim.so", port=2395))
print(y5sh.run(r"""
pkill -9 ccci_mdinit; sleep 1
chroot /mnt/root python3 /tmp/ci2.py 2>&1 | tail -n 2
dmesg -c >/dev/null
( chroot /aroot /system/bin/sh -c "LD_PRELOAD=/data/propshim.so exec /vendor/bin/ccci_mdinit 0" >/tmp/mdinit.out 2>&1 </dev/null & )
sleep 55
cat /sys/kernel/ccci/boot | cut -c1-6
dmesg | grep -i -E 'md_state|HS2|exception|assert|filename|line =|ready|rat|md_boot' | grep -v rbf | head -n 25
""", timeout=100))
