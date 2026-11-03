#!/bin/sh
set -eu
cd /opt/microfw/ui
cc -O2 -std=c11 -Wall -Wextra -Werror -Isrc $(pkg-config --cflags cairo) tools/bench_sample.c src/model.c -o build/bench-sample
./build/bench-sample
