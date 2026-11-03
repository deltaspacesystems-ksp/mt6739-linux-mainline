#!/bin/sh
# Start systemu z flash (po switch_root z initramfs). Gadzet USB i usb0 sa juz skonfigurowane przez initramfs.
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
mount -t proc proc /proc 2>/dev/null
mount -t sysfs sysfs /sys 2>/dev/null
mount -t devtmpfs devtmpfs /dev 2>/dev/null
mount -t debugfs none /sys/kernel/debug 2>/dev/null
mkdir -p /dev/pts /tmp /run
mount -t devpts devpts /dev/pts 2>/dev/null
mount -t tmpfs tmpfs /tmp 2>/dev/null
wdt x 0x10007000 0x22000000
hostname microfw
ip link set lo up
ip addr show usb0 2>/dev/null | grep -q 192.168.7.2 || { ip addr add 192.168.7.2/24 dev usb0; ip link set usb0 up; }
killall udhcpd telnetd 2>/dev/null
touch /tmp/udhcpd.leases
udhcpd /etc/udhcpd.conf &
telnetd -b 192.168.7.2 -l /bin/sh &
echo "rc.sh ok $(date)" > /tmp/rc.log

# microfw POSIX shm: required by Wayland/wlroots keymaps and clients.
mkdir -p /dev/shm
mountpoint -q /dev/shm || mount -t tmpfs -o mode=1777,nosuid,nodev tmpfs /dev/shm
