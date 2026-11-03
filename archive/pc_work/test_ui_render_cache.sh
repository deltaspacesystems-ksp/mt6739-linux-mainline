#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work/microfw-ui
cc -O2 -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo pangocairo) -Dmfw_render=mfw_render_reference -c tools/render_reference_before_cache.c -o build/render-reference.o
cc -O2 -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo pangocairo) tools/test_render_cache.c src/render.c build/icons.c build/render-reference.o -o build/test-render-cache $(pkg-config --libs cairo pangocairo) -lm
./build/test-render-cache
