#!/bin/sh
# Bounded trial. PMIC unbound ONLY around short power on/off phases.
set -eu
umask 077
root=/var/cache/mfw-ui
duration=${1:-600}
case "$duration" in ''|*[!0-9]*) exit 2;; esac
[ "$duration" -ge 1 ] && [ "$duration" -le 600 ] || exit 2
lock=/run/mfw-touch.lock
node=/sys/bus/platform/devices/1000d000.pwrap
driver=/sys/bus/platform/drivers/y5-pmic-monitor
mkdir "$lock"
reader=
db=
tag=
rebind() { [ -L "$node/driver" ] || printf 1000d000.pwrap > "$driver/bind"; }
power() {
 test "$(basename "$(readlink "$node/driver")")" = y5-pmic-monitor || return 1
 printf 1000d000.pwrap > "$driver/unbind" || return 1
 result=0
 timeout 3 "$root/y5_mstar_touch_power" "$1" || result=$?
 rebind || result=1
 return "$result"
}
cleanup() {
 result=$?
 trap - EXIT HUP INT TERM
 set +e
 if [ -n "$reader" ]; then kill -TERM "$reader" 2>/dev/null; wait "$reader" 2>/dev/null; fi
 [ -z "$db" ] || rm -f -- "$db"
 [ -z "$tag" ] || rm -f -- "$tag"
 rebind || result=1
 if [ -f "$lock/active" ]; then power --off || { echo 'TOUCH_RESTORE_FAILED â€” keep lease for recovery'; result=1; }; fi
 rebind || result=1
 [ -e "$lock/active" ] || rmdir "$lock"
 printf 'EXIT %s\n' "$result" > "$root/touch.status"
 exit "$result"
}
trap cleanup EXIT
trap 'exit 129' HUP
trap 'exit 130' INT
trap 'exit 143' TERM
printf '%s\n' "$$" > "$root/touch-supervisor.pid"
power --on
test -L "$node/driver"
printf 'RUNNING %ss\n' "$duration" > "$root/touch.status"
"$root/mfw_mstar_touch" --seconds "$duration" > "$root/touch-reader.log" 2>&1 &
reader=$!
n=0
while ! grep -q '^UI_SYSNAME=' "$root/touch-reader.log" && [ "$n" -lt 40 ]; do
 kill -0 "$reader" || exit 5
 n=$((n+1)); sleep .1
done
name=$(sed -n 's/^UI_SYSNAME=//p' "$root/touch-reader.log")
case "$name" in input[0-9]*) ;; *) exit 5;; esac
for path in /sys/devices/virtual/input/"$name"/event*/dev; do
 [ -f "$path" ] || continue
 devno=$(cat "$path")
 case "$devno" in 13:[0-9]*) ;; *) exit 5;; esac
 # This minimal rootfs has libudev but no udev daemon. Classify ONLY our
 # newly created direct-touch uinput device before starting labwc.
 target=/run/udev/data/c$devno
 target_tag=/run/udev/tags/seat/c$devno
 test ! -e "$target" && test ! -e "$target_tag"
 mkdir -p /run/udev/data /run/udev/tags/seat
 printf 'E:ID_INPUT=1\nE:ID_INPUT_TOUCHSCREEN=1\nE:ID_SEAT=seat0\nG:seat\nQ:seat\n' > "$target"
 db=$target
 : > "$target_tag"
 tag=$target_tag
 printf 'TOUCH_DEVICE=%s/%s dev=%s\n' "$name" "$(basename "$(dirname "$path")")" "$devno"
 break
done
test -n "$db"
wait "$reader"
reader=
