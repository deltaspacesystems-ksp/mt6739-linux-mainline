(
for pid in $(pidof labwc 2>/dev/null); do
 [ "$(readlink /proc/$pid/exe)" = /usr/bin/labwc ] || continue
 cmd=$(tr '\000' ' ' < /proc/$pid/cmdline)
 case "$cmd" in *'/var/cache/mfw-ui/config'*) kill -TERM "$pid"; echo "Stopped owned labwc PID=$pid";; esac
done
printf 0 > /sys/kernel/y5disp/hz
)
