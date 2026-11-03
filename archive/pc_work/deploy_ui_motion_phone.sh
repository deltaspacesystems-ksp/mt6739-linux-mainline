#!/bin/sh
set -eu
cd /opt/microfw/ui
[ ! -S /run/seatd.sock ]
cp -p build/microfw-shell build/microfw-shell.pre-motion
 tar -xzf /var/cache/mfw-ui/ui-motion-update.tar.gz -C /opt/microfw/ui
cc -Isrc -Ibuild $(pkg-config --cflags cairo pangocairo wayland-client) -O2 -g -pthread -std=c11 -Wall -Wextra -Werror src/main.c src/motion.c src/model.c src/sampler.c src/render.c src/wayland.c src/desktop.c src/gestures.c build/icons.c build/xdg-shell-protocol.c build/xdg-decoration-protocol.c build/wlr-layer-shell-protocol.c build/wlr-foreign-toplevel-management-protocol.c -o build/microfw-shell.motion $(pkg-config --libs cairo pangocairo wayland-client) -lm
mv build/microfw-shell.motion build/microfw-shell
printf 'MOTION_DEPLOYED\n'
