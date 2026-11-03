import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- uname / vermagic kernela:'; uname -r
echo '--- vermagic w module wlan:'; strings /ro/vendor/lib/modules/wlan_drv_gen2.ko | grep -m1 vermagic
echo '--- vermagic wmt_drv:'; strings /ro/vendor/lib/modules/wmt_drv.ko | grep -m1 vermagic
echo '--- lista modulow vendor:'; ls -la /ro/vendor/lib/modules/
echo '--- moduly juz zaladowane:'; lsmod 2>&1 | head
echo '--- insmod wmt_drv (test):'; insmod /ro/vendor/lib/modules/wmt_drv.ko 2>&1
echo '--- dmesg po probie:'; dmesg | tail -n 15
""", timeout=40))
