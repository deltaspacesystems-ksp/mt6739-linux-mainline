import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg -c >/dev/null
( chroot /aroot /vendor/bin/ccci_mdinit 0 >/tmp/mdinit.out 2>&1 </dev/null & ); sleep 15
cat /tmp/mdinit.out | head; cat /sys/kernel/ccci/boot
echo --- dmesg; dmesg | grep -i -E 'ccci|md1|\bmd\b|modem|ccmni|exception|EE' | grep -v VcoreFS | head -n 50
echo --- log; strings /mnt/root/tmp/logdw.txt | tail -n 20
""", timeout=60))
