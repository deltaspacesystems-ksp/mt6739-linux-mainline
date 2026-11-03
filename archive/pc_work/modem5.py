import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /aroot/vendor/etc; ls | grep -i fstab; grep -v '^#' fstab.mt6739 | grep -i -E 'nvdata|nvcfg|protect|persist|nvram|md1' 
grep -n -i -E 'nvram_daemon|nvdata|protect' init/hw/init.mt6739.rc | head -n 30
ls -l /aroot/vendor/bin/nvram_daemon /aroot/vendor/bin/ccci_mdinit /aroot/vendor/bin/ccci_fsd
cat /sys/kernel/ccci/boot; cat /proc/ccci_dump 2>/dev/null | head -n 5
""", timeout=40))
