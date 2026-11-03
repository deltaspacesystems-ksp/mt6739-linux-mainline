#!/bin/sh
set -eu
umask 077
root=/var/cache/mfw-ui
node=/sys/bus/platform/devices/1400b000.disp_rdma0
status=$root/dma-flip.status
test ! -S /run/seatd.sock
test ! -e /run/mfw-touch.lock/active
test -w "$node/flip_test"
result=1
cleanup() {
 printf 'RESULT %s\n' "$result" > "$status"
 cat "$node/flip_state" "$node/counters"
 dmesg | grep -E 'DMA test buffer|DMA flip test' | tail -n 8
}
trap cleanup EXIT
printf 'COUNTDOWN 5s then green5s magenta5s original\n' > "$status"
cat "$node/counters"
sleep 5
printf 'RUNNING\n' > "$status"
if printf 'run\n' > "$node/flip_test"; then result=0; else result=$?; fi
exit "$result"
