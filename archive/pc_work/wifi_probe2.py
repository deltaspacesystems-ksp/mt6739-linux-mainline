import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mkdir -p /ro/system /ro/vendor
mountpoint -q /ro/vendor || mount -t ext4 -o ro /dev/part_vendor /ro/vendor
mountpoint -q /ro/system || mount -t ext4 -o ro /dev/part_system /ro/system
mount | grep /ro/
echo '--- vermagic wlan:'; strings /ro/vendor/lib/modules/wlan_drv_gen2.ko | grep -m1 vermagic
echo '--- vermagic wmt_drv:'; strings /ro/vendor/lib/modules/wmt_drv.ko | grep -m1 vermagic
echo '--- insmod wmt_drv:'; insmod /ro/vendor/lib/modules/wmt_drv.ko 2>&1
echo '--- dmesg tail:'; dmesg | tail -n 12
""", timeout=40))
