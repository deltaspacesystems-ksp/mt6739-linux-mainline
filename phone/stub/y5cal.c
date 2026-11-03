typedef unsigned long u64; typedef unsigned int u32; typedef unsigned char u8;
static long sc(long n, long a, long b, long c, long d, long e, long f)
{
	register long x8 asm("x8") = n; register long x0 asm("x0") = a; register long x1 asm("x1") = b;
	register long x2 asm("x2") = c; register long x3 asm("x3") = d; register long x4 asm("x4") = e; register long x5 asm("x5") = f;
	asm volatile("svc 0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "memory");
	return x0;
}
void *memset(void *d, int c, unsigned long n) { unsigned char *p = d; while (n--) *p++ = (unsigned char)c; return d; }
void *memcpy(void *d, const void *s, unsigned long n) { unsigned char *p = d; const unsigned char *q = s; while (n--) *p++ = *q++; return d; }
#define W 720
#define H 1440
#define COLS 90
#define ROWS 90
static u32 buf[W * H];
int cmain(long *sp)
{
	long argc = sp[0]; char **argv = (char **)(sp + 1);
	u32 solid = 0; int use = 0;
	if (argc > 1) { for (char *p = argv[1]; *p; p++) { int d = *p <= '9' ? *p - '0' : (*p | 32) - 'a' + 10; solid = solid * 16 + d; } use = 1; }
	for (int i = 0; i < W * H; i++) buf[i] = 0xff000000;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			u32 c = 0xff000000;
			if (x % 100 == 0 && x) c = 0xff00ff00;
			if (y % 100 < 3) { c = 0xffffffff; if (x < (y / 100 + 1) * 60 && x > 10) c = 0xffff0000; }
			if (x < 8) c = 0xffffffff;
			if (x >= W - 8) c = 0xff00ffff;
			if (y < 16) c = x < W / 4 ? 0xffff0000 : x < W / 2 ? 0xff00ff00 : x < 3 * W / 4 ? 0xff0000ff : 0xffffff00;
			buf[y * W + x] = c;
		}
	}
	if (use) for (int i = 0; i < W * H; i++) buf[i] = solid;
	long mfd = sc(56, -100, (long)"/dev/mem", 0x101002, 0, 0, 0);
	if (mfd < 0) return 3;
	volatile u32 *fb = (volatile u32 *)((long)sc(222, 0, 0x3f7000, 3, 1, mfd, 0x775e8000) + 0xe38);
	if ((long)fb < 0 && (long)fb > -4096) return 4;
	for (int i = 0; i < W * H; i++) fb[i] = buf[i];
	volatile u32 *mtx = (volatile u32 *)sc(222, 0, 0x1000, 3, 1, mfd, 0x14015000);
	if (!((long)mtx < 0 && (long)mtx > -4096)) mtx[0x20 / 4] = 1;
	volatile u32 *rdma = (volatile u32 *)sc(222, 0, 0x1000, 3, 1, mfd, 0x1400b000);
	if (!((long)rdma < 0 && (long)rdma > -4096)) rdma[0x10 / 4] = 1;
	volatile u32 *dsi = (volatile u32 *)sc(222, 0, 0x1000, 3, 1, mfd, 0x14012000);
	if (!((long)dsi < 0 && (long)dsi > -4096)) { dsi[0] = 0; dsi[0] = 1; }
	return 0;
}
asm(".global _start\n_start:\n mov x0, sp\n bl cmain\n mov x8, #93\n svc 0\n");
