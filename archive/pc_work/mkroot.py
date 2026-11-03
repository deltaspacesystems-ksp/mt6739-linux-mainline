import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
apk add --no-cache e2fsprogs util-linux 2>&1 | tail -n 3
DEV=""
for b in /sys/class/block/*; do grep -qx 'PARTNAME=userdata' $b/uevent 2>/dev/null && DEV=$b; done
echo dev=$DEV size_sectors=$(cat $DEV/size)
MM=$(cat $DEV/dev); [ -e /dev/part_userdata ] || mknod /dev/part_userdata b ${MM%:*} ${MM#*:}
mkdir -p /mnt/root
mkfs.ext4 -F -q -L y5root -E lazy_itable_init=1,lazy_journal_init=1 /dev/part_userdata 2>&1 | tail -n 3
mount -t ext4 /dev/part_userdata /mnt/root && echo MOUNT_OK
df -h /mnt/root | tail -n 1
free -m | sed -n 2p
"""
print(y5sh.run(cmd, timeout=600))
