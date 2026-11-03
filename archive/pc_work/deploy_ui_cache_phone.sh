#!/bin/sh
set -eu
cd /opt/microfw/ui
test "$(cat /var/cache/mfw-ui/ui-update.status)" = READY
test -f build/microfw-shell.next
if [ ! -e build/microfw-shell.pre-cache ]; then cp -p build/microfw-shell build/microfw-shell.pre-cache; fi
mv -f build/microfw-shell.next build/microfw-shell
printf 'DEPLOYED\n' > /var/cache/mfw-ui/ui-update.status
