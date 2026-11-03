#!/bin/bash
set -eu
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.."
export XDG_RUNTIME_DIR=/tmp/microfw-labwc-runtime
mkdir -p "$XDG_RUNTIME_DIR" build/labwc-config
chmod 700 "$XDG_RUNTIME_DIR"
printf '<labwc_config/>\n' > build/labwc-config/rc.xml
rm -f build/desktop-test.pass
printf -v session_command 'bash %q' "$PWD/tools/labwc_session.sh"
WLR_BACKENDS=headless WLR_HEADLESS_OUTPUTS=1 WLR_RENDERER=pixman WLR_LIBINPUT_NO_DEVICES=1 \
    timeout 25s labwc -C build/labwc-config -S "$session_command" > build/labwc-test.log 2>&1
test -f build/desktop-test.pass
grep -E '^DESKTOP_CONFIGURED|^SMOKE_' build/desktop-test.log
printf 'PASS: layer-shell desktop and two real clients, fullscreen/restore/minimize/activate/close acknowledged by compositor\n'
