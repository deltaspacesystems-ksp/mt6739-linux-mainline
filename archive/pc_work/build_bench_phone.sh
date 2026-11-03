#!/bin/sh
set -eu
cd /opt/microfw/ui
cc -std=c11 -O2 -Wall -Wextra -Werror -Isrc tools/bench_render.c src/render.c build/icons.c -o build/bench-render $(pkg-config --cflags --libs cairo pangocairo) -lm
./build/bench-render
