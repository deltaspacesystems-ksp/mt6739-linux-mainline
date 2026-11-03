#!/bin/sh
# Read-only inventory; no device activation and no raw device-tree dump.
echo CAPTURE_NODES
for pattern in /dev/video* /dev/media* /dev/snd/* /dev/i2c-* /dev/gnss* /dev/stpgps*; do
    [ ! -e "$pattern" ] || printf '%s\n' "$pattern"
done
echo ALSA_CARDS
[ ! -f /proc/asound/cards ] || cat /proc/asound/cards
echo SENSOR_NODES
find /sys/firmware/devicetree/base -type d | grep -Ei 'ltr|als|camera|imgsensor|mic|audio|gps|mc3|flashlight|seninf'
echo KERNEL_INTERFACES
for path in /sys/bus/i2c/devices /sys/bus/iio/devices /sys/class/video4linux /sys/class/sound /sys/class/gnss; do
    [ ! -d "$path" ] || ls "$path"
done
echo DONE
