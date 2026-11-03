cd /mnt/d/y5_linux_work
aarch64-linux-gnu-gcc -static -nostdlib -O2 -ffreestanding -fno-tree-loop-distribute-patterns -o out/y5cal y5cal.c 2>&1 | head -8
aarch64-linux-gnu-gcc -static -nostdlib -O2 -ffreestanding -fno-tree-loop-distribute-patterns -o out/y5txt y5txt.c 2>&1 | head -8
ls -l out/y5cal out/y5txt
aarch64-linux-gnu-objdump -d out/y5txt | sed -n '/<memset>:/,/<memcpy>:/p' | head -12
