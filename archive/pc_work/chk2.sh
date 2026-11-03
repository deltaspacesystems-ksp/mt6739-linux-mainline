cd /home/y5/linux-7.3
grep -n 'start_kernel(void)' init/main.c | head
grep -n '^void panic\|^static void panic\|^void __weak panic\|panic(const char' kernel/panic.c | head -5
sed -n '/^void panic(/,+3p' kernel/panic.c | head -6
grep -n 'include <linux/' kernel/panic.c | head -3
