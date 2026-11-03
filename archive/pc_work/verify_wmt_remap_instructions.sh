#!/bin/bash
set -eu
aarch64-linux-gnu-objdump -d --start-address=0x2e58 --stop-address=0x2f50 /mnt/d/y5_linux_work/wmt_drv.ko > /mnt/d/y5_linux_work/out/wmt-remap-reset-disassembly.txt
cat /mnt/d/y5_linux_work/out/wmt-remap-reset-disassembly.txt | tail -38
