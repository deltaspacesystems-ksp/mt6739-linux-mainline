#!/bin/bash
set -e
mkdir -p /home/y5/stub && cd /home/y5/stub
cp /mnt/c/Users/pc/Documents/y5_linux_work/payload.S .
aarch64-linux-gnu-as -o payload.o payload.S
aarch64-linux-gnu-ld -nostdlib -Ttext=0 -o payload.elf payload.o
aarch64-linux-gnu-objcopy -O binary payload.elf payload.bin
ls -l payload.bin
cp payload.bin /mnt/d/y5_linux_work/out/payload.bin
aarch64-linux-gnu-objdump -d payload.elf | head -22
