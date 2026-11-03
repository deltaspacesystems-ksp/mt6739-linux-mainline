#!/bin/bash
set -eu
aarch64-linux-gnu-objdump -d --disassemble=PVRUniFlexGetTileBufferSize /mnt/d/y5_linux_work/private_gpu_reference/libusc.so 2>/dev/null | tail -8
