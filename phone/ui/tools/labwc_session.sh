#!/bin/bash
set -eu
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.."
./build/microfw-shell --desktop --demo --frames 8 --smoke-actions > build/desktop-test.log 2>&1 &
desktop_pid=$!
weston-terminal > build/terminal-test.log 2>&1 &
terminal_pid=$!
./build/microfw-shell --page settings --demo > build/settings-test.log 2>&1 &
settings_pid=$!
trap 'kill "$desktop_pid" "$terminal_pid" "$settings_pid" 2>/dev/null || true; wait 2>/dev/null || true' EXIT
wait "$desktop_pid"
grep -q SMOKE_CLOSE_ACK_PASS build/desktop-test.log
printf 'PASS\n' > build/desktop-test.pass
