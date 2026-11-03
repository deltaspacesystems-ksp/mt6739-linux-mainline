#!/bin/sh
set -eu
/usr/local/bin/mfw-light-start
out=/tmp/mfw-light-response.log
read start rest < /proc/uptime
start=${start%.*}
end=$((start+45))
: > "$out"
while :; do
 read now rest < /proc/uptime
 now=${now%.*}
 [ "$now" -lt "$end" ] || break
 als=$(cat /sys/bus/iio/devices/iio:device0/in_illuminance_raw)
 aux=$(cat /sys/bus/iio/devices/iio:device0/in_intensity0_channel0a_raw)
 printf 't=%s als=%s channel0a=%s\n' "$((now-start))" "$als" "$aux" >> "$out"
 sleep 0.2
done
echo COMPLETE >> "$out"
