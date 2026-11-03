#!/bin/sh
set -eu
umask 077
root=/var/cache/mfw-ui
node=/sys/bus/platform/devices/10006000.mfw-gpu-id-probe
pmic=/sys/bus/platform/devices/1000d000.pwrap
mkdir /run/mfw-gpu-probe.lock
result=1
cleanup() {
 printf 'RESULT %s\n' "$result" > "$root/gpu-id.status"
 cat "$node/identity" 2>/dev/null || true
 dmesg | grep -E 'GPU ID probe|GPU raw|Power-up stopped|Power-down stopped|MFG CCF|VCORE readonly' | tail -n 12
 rmdir /run/mfw-gpu-probe.lock
}
trap cleanup EXIT
test ! -S /run/seatd.sock
test ! -e /run/mfw-touch.lock/active
test -w "$node/probe_id"
cat "$pmic/vcore_status"
voltage=$(sed -n 's/.*voltage_uv=\([0-9]*\).*/\1/p' "$pmic/vcore_status")
case "$voltage" in ''|*[!0-9]*) exit 3;; esac
[ "$voltage" -ge 1050000 ] && [ "$voltage" -le 1312500 ] || exit 3
printf 'RUNNING bounded26MHz identity probe\n' > "$root/gpu-id.status"
if printf 'read-id\n' > "$node/probe_id"; then result=0; else result=$?; fi
exit "$result"
