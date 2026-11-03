import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -ld /aroot/vendor/nvdata /aroot/vendor/nvcfg /aroot/vendor/protect_f /aroot/vendor/protect_s /aroot/mnt /aroot/nvram /aroot/data/nvram 2>&1
ls /sys/class/block | grep -c mmcblk0p; ls /dev/block 2>&1 | head -n 3; ls /dev | grep mmcblk | head -n 3
ls /aroot/system/etc/init | head -n 3
""", timeout=40))
