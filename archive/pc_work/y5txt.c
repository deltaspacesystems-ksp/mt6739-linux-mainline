#include "font8x16.h"
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
static u8 txt[65536];
static char lines[ROWS * 4][COLS + 1];
int cmain(long *sp)
{
	long argc = sp[0]; char **argv = (char **)(sp + 1);
	if (argc < 2) return 1;
	long fd = sc(56, -100, (long)argv[1], 0, 0, 0, 0);
	if (fd < 0) return 2;
	long n = sc(63, fd, (long)txt, sizeof(txt) - 1, 0, 0, 0);
	sc(57, fd, 0, 0, 0, 0, 0);
	if (n < 0) n = 0;
	/* podziel na wiersze (zawijanie przy COLS) */
	int nl = 0, col = 0;
	for (int i = 0; i < ROWS * 4; i++) for (int j = 0; j <= COLS; j++) lines[i][j] = 0;
	for (long i = 0; i < n; i++) {
		u8 ch = txt[i];
		if (ch == '\n' || col >= COLS) {
			nl++; col = 0;
			if (ch == '\n') continue;
		}
		if (nl >= ROWS * 4) break;
		if (ch < 32 || ch > 126) ch = '?';
		lines[nl][col++] = ch;
	}
	nl++;
	int first = nl > ROWS ? nl - ROWS : 0;
	for (int i = 0; i < W * H; i++) buf[i] = 0xff000000;
	for (int r = 0; r < ROWS && first + r < nl; r++) {
		char *ln = lines[first + r];
		for (int c = 0; c < COLS && ln[c]; c++) {
			const u8 *g = &font8x16[(u8)ln[c] * 16];
			for (int y = 0; y < 16; y++)
				for (int x = 0; x < 8; x++)
					if (g[y] & (0x80 >> x))
						buf[(r * 16 + y) * W + c * 8 + x] = 0xffffffff;
		}
	}
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
