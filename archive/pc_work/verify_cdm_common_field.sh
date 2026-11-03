#!/bin/bash
set -eu
aarch64-linux-gnu-objdump -d --start-address=0x30044 --stop-address=0x300a0 /mnt/d/y5_linux_work/private_gpu_reference/libsrv_um.so
