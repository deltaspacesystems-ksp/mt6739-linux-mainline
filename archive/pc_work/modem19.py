import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\shim\propshim.so", "/aroot/data/propshim.so", port=2395))
print(y5sh.run(r"""
pkill ccci_mdinit; sleep 1; rm -f /aroot/data/prop.log; dmesg -c >/dev/null
( chroot /aroot /system/bin/sh -c 'LD_PRELOAD=/data/propshim.so exec /vendor/bin/ccci_mdinit 0' >/tmp/mdinit.out 2>&1 </dev/null & ); sleep 20
sort /aroot/data/prop.log | uniq -c | head -n 30
cat /sys/kernel/ccci/boot; ps | grep mdinit | grep -v grep
dmesg | grep -i -E 'ccci|md1|modem|ccmni|MD_|exception|rf' | grep -v -E 'VcoreFS|charger|Power/' | tail -n 40
""", timeout=70))
