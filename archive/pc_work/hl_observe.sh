#!/bin/sh
set -eu
i=0
while [ "$i" -lt 14 ]; do
 printf 'uptime='
 cut -d' ' -f1 /proc/uptime
 cat /sys/bus/platform/devices/1000d000.pwrap/charger_registers
 i=$((i+1))
 sleep 5
done