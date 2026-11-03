#!/bin/sh
set -eu
status=/var/cache/mfw-ui/build.status
printf 'RUNNING\n' > "$status"
trap 'result=$?; if [ "$result" -ne 0 ]; then printf "FAILED %s\n" "$result" > "$status"; fi' EXIT
test "$(sha256sum /var/cache/mfw-ui/source.tar.gz | cut -d' ' -f1)" = cfc245295ac9a66aee07acab31d5c3f70ed96a93147d8febba318c67bf9465c9
tar -xzf /var/cache/mfw-ui/source.tar.gz -C /opt/microfw
cd /opt/microfw/ui
make -j2 all
make build/test-cpp
./build/test-cpp
./build/microfw-shell --preview /var/cache/mfw-ui/native-desktop.png --width 720 --height 1440
file ./build/microfw-shell
printf 'READY\n' > "$status"
