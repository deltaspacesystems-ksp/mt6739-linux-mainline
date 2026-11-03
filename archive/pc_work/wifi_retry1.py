import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
lsmod | grep -q wmt_drv || insmod /ro/vendor/lib/modules/wmt_drv.ko 2>&1
lsmod | grep -q wmt_chrdev_wifi || insmod /ro/vendor/lib/modules/wmt_chrdev_wifi.ko 2>&1
lsmod | grep -q wlan_drv_gen2 || insmod /ro/vendor/lib/modules/wlan_drv_gen2.ko 2>&1
lsmod
""", timeout=30))
