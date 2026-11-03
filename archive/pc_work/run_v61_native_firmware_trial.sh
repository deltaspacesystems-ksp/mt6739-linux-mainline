#!/bin/sh
# Explicit post-boot bind/preparation only. CPU userspace remains native.
# DDK loads/starts firmware on the first render-node OPEN, done separately
# with the native C probe only after inspecting this bind result.
# Reads only the named GPU firmware from vendor, mounted ext4 ro,noload.
set -eu
umask 077
root=/tmp/mfw-gpu-runtime
mountpoint=$root/vendor-ro
firmware=$root/firmware
node=/sys/bus/platform/devices/13000000.gpu
enable=/sys/module/pvrsrvkm/parameters/enable
pmic=/sys/bus/platform/devices/1000d000.pwrap
mkdir /run/mfw-gpu-runtime.lock
mkdir -p "$root"
cleanup() {
 rc=$?
 if grep -q " $mountpoint " /proc/mounts; then umount "$mountpoint"; fi
 rmdir "$mountpoint" 2>/dev/null || true
 if [ -w "$enable" ] && [ ! -L "$node/driver" ]; then printf '0\n' > "$enable"; fi
 printf 'RESULT=%s\n' "$rc" > "$root/status"
 dmesg | grep -v 'Kernel command line:' | grep -E 'PVR|RGX|GPU|mfw-pvrsrvkm|[Ff]irmware|MFG|VCORE' | tail -n 80 > "$root/dmesg.log"
 cat "$root/dmesg.log"
 rmdir /run/mfw-gpu-runtime.lock
 return "$rc"
}
trap cleanup EXIT
case "$(uname -v)" in '#46 '*) ;; *) echo 'Refusing unexpected kernel build'; exit 3;; esac
df -T /tmp | tail -n 1 | grep -q tmpfs
test ! -S /run/seatd.sock
test ! -e /run/mfw-touch.lock/active
test ! -e /sys/bus/platform/devices/10006000.mfw-gpu-id-probe/identity
test ! -e "$root/attempted"
test -d "$node" && test -w "$enable"
test ! -L "$node/driver"
grep -q 'enabled=1' "$pmic/vcore_status"
voltage=$(sed -n 's/.*voltage_uv=\([0-9]*\).*/\1/p' "$pmic/vcore_status")
case "$voltage" in ''|*[!0-9]*) exit 3;; esac
[ "$voltage" -ge 1050000 ] && [ "$voltage" -le 1312500 ] || exit 3
test "$(sed -n 's/^PARTNAME=//p' /sys/class/block/mmcblk0p41/uevent)" = vendor
if grep -q '^/dev/mmcblk0p41 ' /proc/mounts; then
 echo 'Vendor already mounted; refusing to change existing mount'; exit 3
fi
mkdir "$mountpoint"
mkdir -p "$firmware"
mount -t ext4 -o ro,noload /dev/mmcblk0p41 "$mountpoint"
source=$mountpoint/firmware/rgx.fw.signed
test "$(stat -c %s "$source")" -eq 1265664
test "$(sha256sum "$source" | cut -d ' ' -f1)" = 8a843eb1678c0e2fd630c66d4a8070a0a6d4ca5ba7bfd16904d89d66f9fa074c
cp "$source" "$firmware/rgx.fw.signed"
umount "$mountpoint"
rmdir "$mountpoint"
test "$(sha256sum "$firmware/rgx.fw.signed" | cut -d ' ' -f1)" = 8a843eb1678c0e2fd630c66d4a8070a0a6d4ca5ba7bfd16904d89d66f9fa074c
printf '%s\n' "$firmware" > /sys/module/firmware_class/parameters/path
cat "$pmic/vcore_status" > "$root/vcore-before"
cat /proc/interrupts > "$root/interrupts-before"
printf 'RUNNING native DDK bind, firmware staged; no first OPEN or rendering jobs\n' > "$root/status"
touch "$root/attempted"
printf '1\n' > "$enable"
printf '13000000.gpu\n' > /sys/bus/platform/drivers_probe
test "$(basename "$(readlink "$node/driver")")" = microfw-pvrsrvkm
cat /proc/interrupts > "$root/interrupts-after"
cat "$pmic/vcore_status" > "$root/vcore-after"
ls /dev/dri
echo 'NATIVE_BIND_RETURNED: inspect kernel firmware result before CONNECT'
