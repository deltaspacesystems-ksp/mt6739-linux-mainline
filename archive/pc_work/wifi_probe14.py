import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- zabij stare demony:'; pkill wmt_launcher; sleep 1
echo '--- sciezki firmware:'; ls /ro/vendor/firmware | grep -i -E 'wifi|ram_code'
echo '--- /ro/system/etc/firmware (jesli jest):'; ls /ro/system/etc/firmware 2>&1 | head -n 10
echo '--- probuj z -m 3 -p /vendor/firmware/:'
chroot /aroot /vendor/bin/wmt_launcher -m 3 -p /vendor/firmware/ &
sleep 3
echo '--- /sys/class/net:'; ls /sys/class/net | grep -v ccmni
echo '--- dmesg tail 25:'; dmesg | tail -n 25
""", timeout=40))
