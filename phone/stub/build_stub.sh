#!/bin/bash
# Buduje mikro-kernel z stub.S i zapisuje jako D:\y5_linux_work\out\stub_Image (surowy Image, jak Image kernela).
set -e
mkdir -p /home/y5/stub && cd /home/y5/stub
cp /mnt/c/Users/pc/Documents/y5_linux_work/stub.S .
aarch64-linux-gnu-as -o stub.o stub.S
aarch64-linux-gnu-ld -nostdlib -Ttext=0 -o stub.elf stub.o
aarch64-linux-gnu-objcopy -O binary stub.elf stub.bin
ls -l stub.bin
echo "--- naglowek (offset 0x38 = magic ARM\\x64):"
od -A x -t x1z -N 72 stub.bin
cp stub.bin /mnt/d/y5_linux_work/out/stub_Image
sha256sum stub.bin | cut -c1-16
aarch64-linux-gnu-objdump -d stub.elf | head -30
