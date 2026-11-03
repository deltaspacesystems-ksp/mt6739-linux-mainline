#!/bin/sh
set -eu
status=/var/cache/mfw-ui/install.status
printf 'RUNNING\n' > "$status"
trap 'result=$?; if [ "$result" -ne 0 ]; then printf "FAILED %s\n" "$result" > "$status"; fi' EXIT
rootdev=$(awk '$2=="/" {print $1}' /proc/mounts)
test "$rootdev" = /dev/mmcblk0p46
test "$(sha256sum /var/cache/mfw-ui/package-bundle.tar | cut -d' ' -f1)" = 1801573254b086bfff10ccd93a23edbdea76133dc5cd076860984ebecfc56e63
mkdir -p /etc/y5
[ -e /etc/y5/apk-world.before-ui ] || cp /etc/apk/world /etc/y5/apk-world.before-ui
tar -xf /var/cache/mfw-ui/package-bundle.tar -C /tmp/mfw-apk
apk --repositories-file /tmp/mfw-ui-repositories --no-network add labwc foot wayland-dev cairo-dev pango-dev wayland-protocols build-base pkgconf font-dejavu seatd
cc --version | head -1
labwc --version
pkg-config --modversion cairo pangocairo wayland-client
printf 'READY\n' > "$status"
