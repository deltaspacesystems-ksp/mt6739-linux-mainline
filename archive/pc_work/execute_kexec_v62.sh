#!/bin/sh
set -u
export PATH=/usr/sbin:/usr/bin:/sbin:/bin
sleep 2
[ "$(cat /sys/module/pvrsrvkm/parameters/enable)" = N ] || exit 1
[ "$(cat /sys/kernel/kexec_loaded)" = 1 ] || exit 1
sync
if ! mount -o remount,ro /; then
 echo 'ABORT root remount-ro failed' > /tmp/mfw-kexec-exec.log
 exit 1
fi
echo 'rootfs read-only; executing kexec' > /tmp/mfw-kexec-exec.log
kexec -e >> /tmp/mfw-kexec-exec.log 2>&1
rc=$?
mount -o remount,rw /
echo "kexec unexpectedly returned rc=$rc" >> /tmp/mfw-kexec-exec.log
exit 1
