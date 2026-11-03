(
set -eu
test ! -e /etc/localtime
test ! -L /etc/localtime
test ! -e /etc/timezone
cp /tmp/mfw-Warsaw.tzif /etc/localtime
chmod 644 /etc/localtime
printf 'Europe/Warsaw\n' > /etc/timezone
sync
date '+%Y-%m-%d %H:%M:%S %Z %z'
cat /proc/uptime
cat /sys/bus/platform/devices/1000d000.pwrap/charger_service
)
