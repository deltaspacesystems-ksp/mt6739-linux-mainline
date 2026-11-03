#!/bin/sh
set -eu
cd /opt/microfw/ui
cc -O2 -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo pangocairo) -Dmfw_render=mfw_render_reference -c tools/render_reference_before_cache.c -o build/render-reference.o
cc -O2 -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo pangocairo) tools/test_render_cache.c src/render.c build/icons.c build/render-reference.o -o build/test-render-cache $(pkg-config --libs cairo pangocairo) -lm
./build/test-render-cache
cc -Isrc -Ibuild $(pkg-config --cflags cairo pangocairo wayland-client) -O2 -g -std=c11 -Wall -Wextra -Werror src/main.c src/model.c src/render.c src/wayland.c src/desktop.c src/gestures.c build/icons.c build/xdg-shell-protocol.c build/xdg-decoration-protocol.c build/wlr-layer-shell-protocol.c build/wlr-foreign-toplevel-management-protocol.c -o build/microfw-shell.next $(pkg-config --libs cairo pangocairo wayland-client) -lm
printf 'READY\n' > /var/cache/mfw-ui/ui-update.status
