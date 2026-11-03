#!/bin/sh
# phone_shell --file feeds an interactive shell; the subshell makes EXIT real.
(
set -eu
D=/sys/bus/platform/drivers/y5-pmic-monitor
restore() { echo 1000d000.pwrap > "$D/bind"; }
test -L "$D/1000d000.pwrap"
trap restore EXIT
echo 1000d000.pwrap > "$D/unbind"
timeout 3 /tmp/y5_thermal_probe t
sleep 1
timeout 3 /tmp/y5_thermal_probe t
)
