typedef unsigned long u64; typedef unsigned int u32;
static long sc(long n, long a, long b, long c, long d, long e, long f)
{
	register long x8 asm("x8") = n; register long x0 asm("x0") = a; register long x1 asm("x1") = b;
	register long x2 asm("x2") = c; register long x3 asm("x3") = d; register long x4 asm("x4") = e; register long x5 asm("x5") = f;
	asm volatile("svc 0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "memory");
	return x0;
}
int cmain(long *sp)
{
	long argc = sp[0]; char **argv = (char **)(sp + 1);
	u32 t = 5;
	if (argc > 1) { t = 0; for (char *p = argv[1]; *p >= '0' && *p <= '9'; p++) t = t * 10 + (*p - '0'); }
	long fd = sc(56, -100, (long)"/dev/mem", 0x101002, 0, 0, 0);
	if (fd < 0) return 2;
	volatile u32 *w = (volatile u32 *)sc(222, 0, 0x1000, 3, 1, fd, 0x10007000);
	if ((long)w < 0 && (long)w > -4096) return 3;
	u32 m = w[0];
	w[1] = (t << 11) | 8;
	w[2] = 0x1971;
	w[0] = 0x22000000 | (m & 0xff) | 1;
	return 0;
}
asm(".global _start\n_start:\n mov x0, sp\n bl cmain\n mov x8, #93\n svc 0\n");
