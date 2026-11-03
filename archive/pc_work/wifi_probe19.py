import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root python3 -c "
f=open('/dev/wmtWifi','wb', buffering=0)
f.write(b'1')
print('napisano 1')
"
sleep 2
echo '--- dmesg tail 20:'; dmesg | tail -n 20
echo '--- net:'; ls /sys/class/net | grep -v ccmni
""", timeout=40))
