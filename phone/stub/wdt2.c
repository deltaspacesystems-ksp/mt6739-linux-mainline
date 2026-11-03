typedef unsigned long u64; typedef unsigned int u32; typedef unsigned char u8;
static long sc(long n, long a, long b, long c, long d, long e, long f)
{
	register long x8 asm("x8") = n; register long x0 asm("x0") = a; register long x1 asm("x1") = b;
	register long x2 asm("x2") = c; register long x3 asm("x3") = d; register long x4 asm("x4") = e; register long x5 asm("x5") = f;
	asm volatile("svc 0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "memory");
	return x0;
}
static u64 num(const char *p)
{
	u64 v = 0; int base = 10;
	if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) { base = 16; p += 2; }
	for (;; p++) {
		int d;
		if (*p >= '0' && *p <= '9') d = *p - '0';
		else if (base == 16 && *p >= 'a' && *p <= 'f') d = *p - 'a' + 10;
		else if (base == 16 && *p >= 'A' && *p <= 'F') d = *p - 'A' + 10;
		else break;
		v = v * base + d;
	}
	return v;
}
static volatile u8 *map(long fd, u64 addr)
{
	long p = sc(222, 0, 0x1000, 3, 1, fd, addr & ~0xfffUL);
	if (p < 0 && p > -4096) return 0;
	return (volatile u8 *)p + (addr & 0xfff);
}
int cmain(long *sp)
{
	long argc = sp[0]; char **argv = (char **)(sp + 1);
	long fd = sc(56, -100, (long)"/dev/mem", 0x101002, 0, 0, 0);
	if (fd < 0) return 2;
	if (argc > 2 && argv[1][0] == 'r') { volatile u8 *p = map(fd, num(argv[2])); return p ? *p : 3; }
	if (argc > 3 && argv[1][0] == 'w') { volatile u8 *p = map(fd, num(argv[2])); if (!p) return 3; *p = (u8)num(argv[3]); return 0; }
	u32 t = argc > 1 ? (u32)num(argv[1]) : 5;
	volatile u32 *w = (volatile u32 *)map(fd, 0x10007000);
	if (!w) return 3;
	u32 m = w[0];
	w[1] = (t << 11) | 8;
	w[2] = 0x1971;
	w[0] = 0x22000000 | (m & 0xff) | 1;
	return 0;
}
asm(".global _start\n_start:\n mov x0, sp\n bl cmain\n mov x8, #93\n svc 0\n");
