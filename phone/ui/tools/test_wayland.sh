#!/bin/bash
set -eu
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.."
export XDG_RUNTIME_DIR=/tmp/microfw-ui-runtime
mkdir -p "$XDG_RUNTIME_DIR"
chmod 700 "$XDG_RUNTIME_DIR"
weston --backend=headless-backend.so --renderer=pixman --width=720 --height=1440 --socket=microfw-ui-test --idle-time=0 --no-config --shell=kiosk-shell.so > build/weston-test.log 2>&1 &
compositor_pid=$!
trap 'kill "$compositor_pid" 2>/dev/null || true; wait "$compositor_pid" 2>/dev/null || true' EXIT
for n in $(seq 1 50); do [ ! -S "$XDG_RUNTIME_DIR/microfw-ui-test" ] || break; sleep .1; done
WAYLAND_DISPLAY=microfw-ui-test timeout 20s ./build/microfw-shell --demo --frames 3 > build/wayland-client-test.log 2>&1
cat build/wayland-client-test.log
if grep -Eiq 'protocol error|invalid object|segmentation|assertion' build/wayland-client-test.log; then exit 1; fi
printf 'PASS: real Wayland xdg-shell configure, shared buffers and three frame callbacks\n'
