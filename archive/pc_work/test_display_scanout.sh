#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work/microfw-display
gcc -std=c11 -Wall -Wextra -Werror -O2 -fsanitize=undefined -fno-sanitize-recover=all test_scanout.c -o ../out/test_scanout
../out/test_scanout
