#!/bin/sh
set -eu
node=/sys/bus/platform/devices/1000d000.pwrap
base=/sys/bus/platform/drivers/y5-pmic-monitor
test "$(basename "$(readlink "$node/driver")")" = y5-pmic-monitor
cat "$node/charger_service"
rebind() { [ -L "$node/driver" ] || printf '1000d000.pwrap' > "$base/bind"; }
trap rebind EXIT HUP INT TERM
printf '1000d000.pwrap' > "$base/unbind"
timeout 3 /var/cache/mfw-ui/y5_mstar_frame_probe --probe-mstar-frame
rebind
trap - EXIT HUP INT TERM
cat "$node/charger_service"
