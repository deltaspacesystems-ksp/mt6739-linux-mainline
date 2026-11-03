#!/bin/sh
# Buduje baze udev dla urzadzen wejsciowych (libinput w wlroots jej wymaga; w Alpine nie ma stalego udevd)
# i wylacza dla libinput surowy ekran typu A - zostaje tylko wirtualny y5-touch-b (protokol B).
mkdir -p /run/udev /dev/input
pkill udevd 2>/dev/null; sleep 0.3
/sbin/udevd --daemon; sleep 1
for d in /sys/class/input/event*; do
  n=$(basename $d); mm=$(cat $d/dev)
  [ -e /dev/input/$n ] || mknod /dev/input/$n c ${mm%%:*} ${mm##*:}
  chmod 666 /dev/input/$n
done
udevadm trigger --subsystem-match=input --action=add
udevadm settle --timeout=10
for d in /sys/class/input/event*; do
  case "$(cat $d/device/name 2>/dev/null)" in
    "huawei,ts_kit"|fake-ts-a) echo "E:LIBINPUT_IGNORE_DEVICE=1" >> /run/udev/data/c$(cat $d/dev);;
  esac
done
pkill udevd
exit 0
