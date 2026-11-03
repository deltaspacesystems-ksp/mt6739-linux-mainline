import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for d in /sys/class/ccci_node/* /sys/class/ccci_md_sta/*; do
  n=$(basename $d); [ -f $d/dev ] || continue; MM=$(cat $d/dev); [ -e /dev/$n ] || mknod /dev/$n c ${MM%:*} ${MM#*:}
done
chmod 666 /dev/ccci_* ; ls /dev | grep -c ccci; ls -l /dev/ccci_fs /dev/ccci_md1_sta 2>&1
pkill ccci_fsd; pkill ccci_rpcd; pkill ccci_mdinit; pkill nvram_daemon; sleep 1
dmesg -c >/dev/null
export PATH=/vendor/bin:/system/bin:$PATH
S="LD_PRELOAD=/data/propshim.so"
( chroot /aroot /system/bin/sh -c "$S exec /vendor/bin/nvram_daemon" >/tmp/nvram.out 2>&1 </dev/null & ); sleep 3
( chroot /aroot /system/bin/sh -c "$S exec /vendor/bin/ccci_fsd 0" >/tmp/fsd.out 2>&1 </dev/null & ); sleep 1
( chroot /aroot /system/bin/sh -c "$S exec /vendor/bin/ccci_rpcd 0" >/tmp/rpcd.out 2>&1 </dev/null & ); sleep 1
( chroot /aroot /system/bin/sh -c "$S exec /vendor/bin/ccci_mdinit 0" >/tmp/mdinit.out 2>&1 </dev/null & ); sleep 25
cat /sys/kernel/ccci/boot
dmesg | grep -i -E 'ccci|md1|modem|ccmni|MD_|exception' | grep -v -E 'VcoreFS|charger|Power/' | tail -n 40
""", timeout=70))
