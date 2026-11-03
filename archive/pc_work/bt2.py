import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for n in stpbt:192 stpgps:191; do [ -e /dev/${n%:*} ] || mknod /dev/${n%:*} c ${n#*:} 0; chmod 666 /dev/${n%:*}; done
echo "nameserver 192.168.1.1" > /mnt/root/etc/resolv.conf; echo "nameserver 8.8.8.8" >> /mnt/root/etc/resolv.conf
chroot /mnt/root sh -c 'apk add --no-cache bluez bluez-deprecated gpsd 2>&1 | tail -n 6; which btattach hciattach hciconfig bluetoothd gpsd'
""", timeout=170))
