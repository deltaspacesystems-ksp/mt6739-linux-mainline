#!/bin/sh
set -eu
uname -a
uptime
printf '\nRDMA observer initial counters\n'
found=0
for p in /sys/bus/platform/devices/*/counters; do
 case "$p" in *1400b000*) found=1; echo "$p"; cat "$p";; esac
done
test "$found" = 1
printf '\nIRQ routing\n'
grep -E 'mfw-rdma|1400b000' /proc/interrupts
sleep 5
printf '\nRDMA observer counters after5s\n'
for p in /sys/bus/platform/devices/*/counters; do
 case "$p" in *1400b000*) cat "$p";; esac
done
printf '\nDisplay and charging\n'
cat /sys/kernel/y5disp/mode /sys/kernel/y5disp/hz
for p in /sys/class/power_supply/*; do
 echo "${p##*/}"
 for f in status online temp; do if [ -f "$p/$f" ]; then printf '%s=' "$f"; cat "$p/$f"; fi; done
done
printf '\nRelevant kernel messages\n'
dmesg | grep -E 'mfw-rdma|scanout observer|y5disp|irq.*fail|IRQ.*fail' | tail -n 20
