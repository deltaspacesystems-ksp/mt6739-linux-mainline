#!/bin/bash
set -u
aarch64-linux-gnu-objdump -d --start-address=0x20c84 --stop-address=0x21050 /mnt/d/y5_linux_work/private_gpu_reference/libsrv_um.so | grep -A4 -B4 '#0x78'

