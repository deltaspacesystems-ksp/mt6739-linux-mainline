import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
y5put.put("/mnt/root/etc/apk/repositories", "https://dl-cdn.alpinelinux.org/alpine/v3.24/main\nhttps://dl-cdn.alpinelinux.org/alpine/v3.24/community\n", 0o644)
print(y5sh.run(r"""
chroot /mnt/root sh -c 'apk update 2>&1 | tail -n 3; apk add --no-cache bluez bluez-deprecated gpsd 2>&1 | tail -n 4; which btattach hciattach hciconfig bluetoothd gpsd'
""", timeout=170))
