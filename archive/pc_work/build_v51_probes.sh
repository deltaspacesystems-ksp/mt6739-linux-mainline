#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work
aarch64-linux-gnu-gcc -static -O2 -Wall -Wextra -Werror -o out/y5_hl7005d_probe y5_hl7005d_probe.c
aarch64-linux-gnu-gcc -static -O2 -Wall -Wextra -Werror -o out/y5_cpu_snapshot y5_cpu_snapshot.c
