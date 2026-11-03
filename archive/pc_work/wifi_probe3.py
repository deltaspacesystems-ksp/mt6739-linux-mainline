import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- insmod wlan_drv_gen2:'; insmod /ro/vendor/lib/modules/wlan_drv_gen2.ko 2>&1
echo '--- insmod wmt_chrdev_wifi:'; insmod /ro/vendor/lib/modules/wmt_chrdev_wifi.ko 2>&1
echo '--- lsmod:'; lsmod
echo '--- /dev wmt:'; ls -la /dev | grep -i -E 'wmt|stp'
echo '--- /proc:'; ls /proc | grep -i -E 'wmt|driver'
echo '--- dmesg tail 25:'; dmesg | tail -n 25
""", timeout=40))
