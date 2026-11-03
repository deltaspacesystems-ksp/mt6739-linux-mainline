#!/bin/bash
set -e
mkdir -p /home/y5/stub && cd /home/y5/stub
cp /mnt/c/Users/pc/Documents/y5_linux_work/stub2.S .
aarch64-linux-gnu-as -o stub2.o stub2.S
aarch64-linux-gnu-ld -nostdlib -Ttext=0 -o stub2.elf stub2.o
aarch64-linux-gnu-objcopy -O binary stub2.elf stub2.bin
truncate -s 21521424 stub2.bin          # ten sam rozmiar po rozpakowaniu co stockowy Image
od -A x -t x1z -N 64 stub2.bin | head -5
ls -l stub2.bin
cp stub2.bin /mnt/d/y5_linux_work/out/stub2_Image
