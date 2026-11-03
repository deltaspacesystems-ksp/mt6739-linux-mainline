#!/bin/sh
set -eu
cd /opt/microfw/ui
test "$(cat /var/cache/mfw-ui/ui-update.status)" = READY
test -f build/microfw-shell.next
test ! -S /run/seatd.sock
if [ ! -e build/microfw-shell.pre-async ]; then cp -p build/microfw-shell build/microfw-shell.pre-async; fi
mv -f build/microfw-shell.next build/microfw-shell
printf 'DEPLOYED_ASYNC\n' > /var/cache/mfw-ui/ui-update.status
