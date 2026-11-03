import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root sh -c 'cat /etc/apk/repositories; apk update 2>&1 | tail -n 4; apk add --no-cache bluez bluez-deprecated gpsd 2>&1 | tail -n 5; which btattach hciattach hciconfig bluetoothd gpsd'
""", timeout=170))
