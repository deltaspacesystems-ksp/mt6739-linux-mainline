#!/bin/sh
set -eu
T=$(cat /sys/class/power_supply/microfw-battery/temp)
test "$T" -ge 100
test "$T" -lt 450
test "$(cat /sys/class/power_supply/microfw-usb/online)" = 1
D=/sys/bus/platform/drivers/y5-pmic-monitor
restore() { echo 1000d000.pwrap > "$D/bind"; }
trap restore EXIT
echo 1000d000.pwrap > "$D/unbind"
timeout 90 /tmp/y5_hl_watchdog_test
echo TEST_FINISHED_BEFORE_REBIND