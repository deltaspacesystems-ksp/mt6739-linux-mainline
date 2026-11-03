#!/bin/sh
# Read-only inventory. No bus transactions, MMIO writes or identifying partitions.
printf '__MFW_INVENTORY_BEGIN__\n'
printf 'KERNEL\n'; uname -a
printf 'PLATFORM_DRIVERS\n'
for d in /sys/bus/platform/devices/*; do
 case "$d" in *i2c*|*gpio*|*pinctrl*|*pwrap*|*pmic*|*mfw*|*clock*|*infracfg*)
  printf '%s -> ' "${d##*/}"
  if [ -L "$d/driver" ]; then readlink "$d/driver"; else printf 'unbound\n'; fi
 esac
done
printf 'I2C_ADAPTERS\n'
for d in /sys/class/i2c-adapter/*; do [ -d "$d" ] || continue; printf '%s ' "${d##*/}"; cat "$d/name"; done
printf 'GPIOCHIPS\n'
for d in /sys/bus/gpio/devices/gpiochip*; do [ -d "$d" ] || continue; printf '%s\n' "${d##*/}"; done
printf 'POWER_SUPPLY\n'
for d in /sys/class/power_supply/*; do [ -d "$d" ] || continue; printf '%s\n' "${d##*/}"; done
printf 'BACKLIGHT_LEDS\n'
for d in /sys/class/backlight/* /sys/class/leds/*; do [ -d "$d" ] || continue; printf '%s\n' "$d"; done
printf 'INPUT\n'; cat /proc/bus/input/devices
printf 'THERMAL\n'
for d in /sys/class/thermal/thermal_zone*; do [ -d "$d" ] || continue; cat "$d/type"; cat "$d/temp"; done
printf 'MEMORY\n'; free -m
printf '__MFW_INVENTORY_END__\n'
