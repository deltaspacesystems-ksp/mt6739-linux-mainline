#!/bin/sh
uname -a
uptime
printf '\nVCORE monitor\n'
cat /sys/bus/platform/devices/1000d000.pwrap/vcore_status
printf '\nGPU idle identity\n'
cat /sys/bus/platform/devices/10006000.mfw-gpu-id-probe/identity
printf '\nDisplay IRQ counters\n'
cat /sys/bus/platform/devices/1400b000.disp_rdma0/counters
printf '\nCharging\n'
cat /sys/class/power_supply/microfw-battery/status /sys/class/power_supply/microfw-battery/temp
printf '\nRelevant boot messages\n'
dmesg | grep -E 'MFG CCF|VCORE readonly|GPU power/ID|mfw-gpu|microfw-vcore|Optional VCORE|defer' | tail -n 18
