#!/bin/sh
set -eu
node=/sys/bus/platform/devices/1000d000.pwrap
base=/sys/bus/platform/drivers/y5-pmic-monitor
test "$(basename "$(readlink "$node/driver")")" = y5-pmic-monitor
cat "$node/charger_service"
rebind() { [ -L "$node/driver" ] || printf '1000d000.pwrap' > "$base/bind"; }
trap rebind EXIT HUP INT TERM
printf '1000d000.pwrap' > "$base/unbind"
timeout 3 /tmp/y5_vtouch_snapshot
rebind
trap - EXIT HUP INT TERM
cat "$node/charger_service"
