#!/bin/bash
set -eu
cd /home/y5/rogue-ddk19-port
aarch64-linux-gnu-ld -r --whole-archive built-in.a --no-whole-archive -o mfw-ddk19-linked.o
aarch64-linux-gnu-nm -u mfw-ddk19-linked.o > /mnt/d/y5_linux_work/out/ddk19-undefined.txt
aarch64-linux-gnu-size mfw-ddk19-linked.o
