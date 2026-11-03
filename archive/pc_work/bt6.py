import sys, datetime
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
now = datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
print(now)
print(y5sh.run(r"""
date -u -s "%s"; date
chroot /mnt/root sh -c 'apk update 2>&1 | tail -n 2; apk add --no-cache bluez bluez-deprecated gpsd 2>&1 | tail -n 4; which btattach hciattach hciconfig bluetoothd gpsd'
""" % now, timeout=170))
