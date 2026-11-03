#!/bin/sh
set -u
log=/var/cache/mfw-ui
status=$log/display.status
test ! -e /run/seatd.sock || exit 4
mkdir -p /run/microfw-runtime /var/cache/mfw-ui/config
chmod 700 /run/microfw-runtime
printf '<labwc_config/>\n' > /var/cache/mfw-ui/config/rc.xml
printf 'RUNNING\n' > "$status"
seatd -g root > "$log/seatd.log" 2>&1 &
seatd_pid=$!
cleanup() {
 kill "$seatd_pid" 2>/dev/null || true
 wait "$seatd_pid" 2>/dev/null || true
 if command -v chvt >/dev/null 2>&1; then chvt 1 2>/dev/null || true; fi
}
trap cleanup EXIT
n=0
while [ ! -S /run/seatd.sock ] && [ "$n" -lt 50 ]; do n=$((n+1)); sleep .1; done
test -S /run/seatd.sock || exit 5
export XDG_RUNTIME_DIR=/run/microfw-runtime
export LIBSEAT_BACKEND=seatd
export WLR_BACKENDS=drm,libinput
export WLR_DRM_DEVICES=/dev/dri/card0
export WLR_RENDERER=pixman
export WLR_LIBINPUT_NO_DEVICES=1
timeout -k 5 45 labwc -C /var/cache/mfw-ui/config -s '/opt/microfw/ui/build/microfw-shell --desktop' > "$log/display.log" 2>&1
result=$?
printf 'EXIT %s\n' "$result" > "$status"
exit 0
