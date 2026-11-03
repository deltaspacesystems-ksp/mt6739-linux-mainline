#!/bin/sh
set -eu
cd /opt/microfw/ui
cc -O2 -pthread -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo) tools/test_sampler.c src/sampler.c -o build/test-sampler
./build/test-sampler
cc -Isrc -Ibuild $(pkg-config --cflags cairo pangocairo wayland-client) -O2 -g -pthread -std=c11 -Wall -Wextra -Werror src/main.c src/model.c src/sampler.c src/render.c src/wayland.c src/desktop.c src/gestures.c build/icons.c build/xdg-shell-protocol.c build/xdg-decoration-protocol.c build/wlr-layer-shell-protocol.c build/wlr-foreign-toplevel-management-protocol.c -o build/microfw-shell.next $(pkg-config --libs cairo pangocairo wayland-client) -lm
cc -O2 -pthread -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo) tools/bench_sampler_read.c src/sampler.c src/model.c -o build/bench-sampler-read
./build/bench-sampler-read
printf 'READY\n' > /var/cache/mfw-ui/ui-update.status
