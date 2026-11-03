#!/bin/sh
set -u
log=/var/cache/mfw-ui
status=$log/display.status
test ! -e /run/seatd.sock || exit 4
mkdir -p /run/microfw-runtime /var/cache/mfw-ui/config
chmod 700 /run/microfw-runtime
printf '<labwc_config/>\n' > /var/cache/mfw-ui/config/rc.xml
old_hz=$(cat /sys/kernel/y5disp/hz)
printf 'RUNNING 600s\n' > "$status"
printf 25 > /sys/kernel/y5disp/hz
seatd -g root > "$log/seatd.log" 2>&1 &
seatd_pid=$!
compositor_pid=
cleanup() {
 if [ -n "$compositor_pid" ]; then kill "$compositor_pid" 2>/dev/null || true; fi
 kill "$seatd_pid" 2>/dev/null || true
 wait "$seatd_pid" 2>/dev/null || true
 printf '%s' "$old_hz" > /sys/kernel/y5disp/hz
 if command -v chvt >/dev/null 2>&1; then chvt 1 2>/dev/null || true; fi
}
trap cleanup EXIT
n=0
while [ ! -S /run/seatd.sock ] && [ "$n" -lt 50 ]; do n=$((n+1)); sleep .1; done
test -S /run/seatd.sock || exit 5
export XDG_RUNTIME_DIR=/run/microfw-runtime LIBSEAT_BACKEND=seatd
export WLR_BACKENDS=drm,libinput WLR_DRM_DEVICES=/dev/dri/card0
export WLR_RENDERER=pixman WLR_LIBINPUT_NO_DEVICES=1
timeout -k 5 600 labwc -d -C /var/cache/mfw-ui/config -s '/opt/microfw/ui/build/microfw-shell --desktop' > "$log/display.log" 2>&1 &
compositor_pid=$!
sleep 10
/tmp/y5_fb_snapshot > "$log/fb-before-periodic.txt"
if kill -0 "$compositor_pid" 2>/dev/null; then
 printf 25 > /sys/kernel/y5disp/hz
 sleep 5
 /tmp/y5_fb_snapshot > "$log/fb-after-periodic.txt"
fi
wait "$compositor_pid"
result=$?
printf 'EXIT %s\n' "$result" > "$status"
exit 0
