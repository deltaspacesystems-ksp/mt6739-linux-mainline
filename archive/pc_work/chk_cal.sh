cd /mnt/d/y5_linux_work
aarch64-linux-gnu-gcc -static -nostdlib -O2 -o out/y5cal y5cal.c 2>&1 | head -8
ls -l out/y5cal
