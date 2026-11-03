import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
export PATH=/vendor/bin:/system/bin:/system/xbin:$PATH
( chroot /aroot /vendor/bin/nvram_daemon >/tmp/nvram.out 2>&1 </dev/null & ); sleep 4
( chroot /aroot /vendor/bin/ccci_fsd 0 >/tmp/fsd.out 2>&1 </dev/null & ); sleep 2
( chroot /aroot /vendor/bin/ccci_rpcd 0 >/tmp/rpcd.out 2>&1 </dev/null & ); sleep 2
ps | grep -E 'nvram_d|ccci_fsd|ccci_rpcd|lg.py' | grep -v grep
cat /tmp/nvram.out /tmp/fsd.out /tmp/rpcd.out | head -n 20
echo --- log; strings /mnt/root/tmp/logdw.txt | tail -n 25
""", timeout=60))
