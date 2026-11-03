import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- stan modulow przed:'; lsmod
echo '--- insmod wlan_drv_gen2 (wmt_chrdev_wifi juz zaladowany):'; insmod /ro/vendor/lib/modules/wlan_drv_gen2.ko 2>&1
echo '--- lsmod po:'; lsmod
echo '--- /sys/class/net:'; ls /sys/class/net
echo '--- dmesg tail 20:'; dmesg | tail -n 20
""", timeout=40))
