#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
aarch64-linux-gnu-gcc -static -O2 -Wall -Wextra -Werror -o out/y5_status_probe y5_status_probe.c
