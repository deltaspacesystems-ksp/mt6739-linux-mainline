#!/bin/sh
# Ekran diagnostyczny (docelowo zastapi go konsola fbcon): stan systemu + koniec dmesg.
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
while true; do
  {
    echo "=== microfw $(uname -r) up $(cut -d' ' -f1 /proc/uptime) rootfs=$(grep ' / ' /proc/mounts | head -n1 | cut -d' ' -f1) ==="
    echo "ip: $(ip -4 -o addr show usb0 2>&1 | tr -s ' ' | cut -d' ' -f4)  mem: $(free -m | awk '/Mem/{print $3"/"$2"M"}')"
    echo "udc: $(cat /sys/class/udc/*/state 2>&1) net: $(ls /sys/class/net | tr '\n' ' ')"
    echo "mmc: $(ls /sys/block | tr '\n' ' ')"
    echo "-----"
    dmesg | tail -n 70 | cut -c1-88
  } > /tmp/scr 2>&1
  y5txt /tmp/scr
  sleep 4
done
