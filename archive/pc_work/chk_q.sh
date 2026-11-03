cd /mnt/d/y5_linux_work
which qemu-aarch64 qemu-aarch64-static 2>&1 | head -2
aarch64-linux-gnu-objdump -d out/y5txt | grep -E "bl.*<(memset|memcpy)>|<memset>:|<cmain>:" | head -12
printf 'hello\nworld\n' > /tmp/t.txt
timeout 10 qemu-aarch64-static out/y5txt /tmp/t.txt; echo "rc=$?"
