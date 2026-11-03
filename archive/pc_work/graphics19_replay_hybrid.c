/* graphics19_replay.c - replay a recorded stock PowerVR UM session (replay.bin from replay_gen.py) on the DDK 1.9
 * kernel port, raw ioctl only. Purpose: decide whether the TA hang of graphics19_draw_trial comes from our client's data
 * (replay renders) or from sequence/KM state (replay hangs too).
 *
 *   graphics19-replay replay.bin [--kick N] [-v]
 *     --kick N   issue KICKTA3D number 1..N (default 0: run everything before the first kick, nothing is sent to the GPU)
 *     -v         print every ioctl
 * Every call is compared with the recorded output (handles, FW addresses); outputs that differ are mapped old->new and
 * substituted in later inputs. CPU mappings are made by PMR handle; before each kick the pages the stock UM wrote with
 * the CPU are restored; at the end the CPU-visible state is compared with the recorded final snapshot. */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

typedef uint8_t u8; typedef uint32_t u32; typedef uint64_t u64;
struct srvkm_cmd { u32 bridge, func; u64 in_ptr, out_ptr; u32 in_size, out_size; };
#define SRVKM_REQ 0xC0206440u
#define MAXMAP 8192
#define MAXREG 128

static u32 map_old[MAXMAP], map_new[MAXMAP]; static int nmap;
static struct { u64 off, len; volatile u32 *base; int writable; } reg[MAXREG]; static int nreg;
static int verbose;

static u32 remap32(u32 v, int *hit) {
	for (int i = 0; i < nmap; i++) if (map_old[i] == v) { *hit = 1; return map_new[i]; }
	return v;
}
static int remap_buf(u8 *b, u32 n, const char *what, u32 seq) {
	int hits = 0;
	for (u32 o = 0; o + 4 <= n; o += 4) {
		u32 v; memcpy(&v, b + o, 4); int h = 0; u32 nv = remap32(v, &h);
		if (h) { memcpy(b + o, &nv, 4); hits++; if (verbose) printf("  seq %u %s +%u: %x -> %x\n", seq, what, o, v, nv); }
	}
	return hits;
}
static void map_add(u32 o, u32 n) {
	for (int i = 0; i < nmap; i++) if (map_old[i] == o) { map_new[i] = n; return; }
	if (nmap < MAXMAP) { map_old[nmap] = o; map_new[nmap] = n; nmap++; }
}
static int find_reg(u64 off) { for (int i = 0; i < nreg; i++) if (reg[i].off == off) return i; return -1; }

static int pp[2] = { -1, -1 };
/* Copy via a pipe: faults on unpopulated sparse/device pages become EFAULT instead of SIGBUS. */
static int safe_cp(void *dst, const void *src, size_t n) {
	if (pp[0] < 0 && pipe(pp)) return -1;
	size_t o = 0;
	while (o < n) {
		size_t c = n - o < 4096 ? n - o : 4096; ssize_t w = write(pp[1], (const u8 *)src + o, c);
		if (w <= 0) return -1;
		ssize_t g = 0;
		while (g < w) { ssize_t r = read(pp[0], (u8 *)dst + o + g, (size_t)(w - g));
			if (r <= 0) { u8 junk[4096]; while (g < w && (r = read(pp[0], junk, (size_t)(w - g))) > 0) g += r; return -1; }
			g += r; }
		o += (size_t)w;
	}
	return 0;
}
/* Experiment patches, MFW_PATCH="c:KMASK:PTROFF:BYTEOFF:VAL64;p:KMASK:REGOFF:BYTEOFF:VAL32;..." (hex).
 * c = bytes inside the buffer behind the kick-input pointer at PTROFF (0xc8 CMDTA, 0xd4 CMD3D);
 * p = 32-bit word in a CPU-mapped GPU page (region = PMR offset), written right before the kick.
 * KMASK bit n-1 selects kick n. */
struct patch { char kind; unsigned mask; unsigned long long a, b, val; };
static struct patch patches[512]; static int npatch;
static void parse_patches(const char *s) {
	while (s && *s && npatch < 512) {
		struct patch *p = &patches[npatch]; unsigned long long m, a, b, v;
		if (sscanf(s, "%c:%llx:%llx:%llx:%llx", &p->kind, &m, &a, &b, &v) == 5 && (p->kind == 'c' || p->kind == 'p')) {
			p->mask = (unsigned)m; p->a = a; p->b = b; p->val = v; npatch++;
		}
		s = strchr(s, ';'); if (s) s++;
	}
}
static void on_alarm(int s) { (void)s; fputs("REPLAY_TIMEOUT (alarm)\n", stderr); _exit(3); }

int main(int argc, char **argv) {
	int maxkick = 0; const char *path = NULL;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--kick") && i + 1 < argc) maxkick = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-v")) verbose = 1;
		else path = argv[i];
	}
	if (!path) { fputs("usage: graphics19-replay replay.bin [--kick N] [-v]\n", stderr); return 2; }
	FILE *f = fopen(path, "rb"); if (!f) { perror(path); return 2; }
	fseek(f, 0, SEEK_END); long fsz = ftell(f); fseek(f, 0, SEEK_SET);
	u8 *file = malloc(fsz); if (fread(file, 1, fsz, f) != (size_t)fsz) { perror("read"); return 2; } fclose(f);
	if (memcmp(file, "MFWRPL1", 7)) { fputs("bad replay file\n", stderr); return 2; }
	int fd = open("/dev/dri/renderD128", O_RDWR | O_CLOEXEC);
	if (fd < 0) { perror("open renderD128"); return 2; }
	for (int want = 8; want <= 9; want++) {   /* the stock UM held two /dev/pvr_sync timelines at fd 8 and 9; kicks refer to them by number */
		int t = open("/dev/pvr_sync", O_RDWR | O_CLOEXEC);
		if (t < 0) { perror("open pvr_sync"); continue; }
		if (t != want) { dup2(t, want); close(t); }
	}
	parse_patches(getenv("MFW_PATCH"));
	signal(SIGALRM, on_alarm); alarm(240);

	u8 *p = file + 8, *end = file + fsz;
	int ioctls = 0, bad = 0, diffs = 0, kicks_done = 0, stopped = 0;
	while (p + 8 <= end) {
		u32 type, len; memcpy(&type, p, 4); memcpy(&len, p + 4, 4); u8 *q = p + 8; p += 8 + len;
		if (type == 1) {
			u32 h[8]; memcpy(h, q, 32); q += 32;
			u32 seq = h[0], br = h[1], fn = h[2], isz = h[3], osz = h[4], np = h[5], kick = h[6];
			u32 ip = (isz + 7) & ~7u, op = (osz + 7) & ~7u;
			const u8 *rin = q, *rexp = q + ip; q += ip + op;
			if (kick && kick > (u32)maxkick) {
				printf("STOP before KICKTA3D #%u (seq %u); run with --kick %u to send it\n", kick, seq, kick);
				stopped = 1; break;
			}
			u8 *in = calloc(1, isz + 8), *out = calloc(1, osz + 8);
			memcpy(in, rin, isz);
			int hits = remap_buf(in, isz, "in", seq);
			for (u32 k = 0; k < np; k++) {
				u32 off, l; memcpy(&off, q, 4); memcpy(&l, q + 4, 4); q += 8;
				u8 *buf = calloc(1, l + 16); memcpy(buf, q, l); q += (l + 7) & ~7u;
				hits += remap_buf(buf, l, "ptr", seq);
				for (int pi = 0; pi < npatch; pi++) if (patches[pi].kind == 'c' && kick && (patches[pi].mask >> (kick - 1) & 1) && patches[pi].a == off
					&& patches[pi].b + 8 <= l) { memcpy(buf + patches[pi].b, &patches[pi].val, 8); printf("patch c: kick %u ptr+%x byte %llx <- %llx\n", kick, off, patches[pi].b, patches[pi].val); }
				u64 a = (u64)(uintptr_t)buf; if (off + 8 <= isz) memcpy(in + off, &a, 8);
			}
			struct srvkm_cmd c = { .bridge = br, .func = fn, .in_ptr = (u64)(uintptr_t)in, .out_ptr = (u64)(uintptr_t)out,
				.in_size = isz, .out_size = osz };
			for (int pi = 0; kick && pi < npatch; pi++) if (patches[pi].kind == 'p' && (patches[pi].mask >> (kick - 1) & 1)) {
				int r = find_reg(patches[pi].a); u32 v = (u32)patches[pi].val;
				if (r < 0 || patches[pi].b + 4 > reg[r].len || safe_cp((void *)((u8 *)reg[r].base + patches[pi].b), &v, 4)) { fprintf(stderr,"patch p FAILED (region %llx); abort before kick\n", patches[pi].a); return 4; }
				else { u32 back = 0xdead, w[16] = { 0 }; safe_cp(&back, (u8 *)reg[r].base + patches[pi].b, 4); safe_cp(w, (u8 *)reg[r].base, 64);
					printf("patch p: kick %u region %llx +%llx <- %x readback %x; page head:", kick, patches[pi].a, patches[pi].b, v, back);
					for (int k = 0; k < 16; k++) { printf(" %x", w[k]); } printf("%s", "\n"); }
			}
			if (kick) { printf("KICK #%u seq %u ...\n", kick, seq); fflush(stdout); }
			int rc = ioctl(fd, SRVKM_REQ, &c); int er = errno;
			ioctls++;
			if (rc) { bad++; printf("seq %u %u/%u: ioctl rc=%d errno=%d\n", seq, br, fn, rc, er); }
			int nd = 0;
			for (u32 o = 0; o + 4 <= osz; o += 4) {
				u32 a, b; memcpy(&a, rexp + o, 4); memcpy(&b, out + o, 4);
				if (a == b) continue;
				if (a >= 0x7f000000u && a != 0xffffffffu) continue;   /* user-space pointers differ by design */
				nd++;
				if (a >= 0x100 && b != 0 && br != 19) map_add(a, b);
				if (verbose || nd <= 4) printf("seq %u %u/%u out+%u: rec %08x now %08x%s\n", seq, br, fn, o, a, b, o + 4 == osz ? " (last word = error?)" : "");
			}
			if (nd) diffs++;
			if (verbose) printf("seq %u %u/%u rc=%d diff=%d substituted=%d\n", seq, br, fn, rc, nd, hits);
			if (kick) { kicks_done++; printf("KICK #%u returned rc=%d\n", kick, rc); fflush(stdout); }
			free(in); free(out);
		} else if (type == 2) {
			u64 off, l; u32 prot, fl; memcpy(&off, q, 8); memcpy(&l, q + 8, 8); memcpy(&prot, q + 16, 4); memcpy(&fl, q + 20, 4);
			u32 hnew = 0, hit = 0; hnew = remap32((u32)(off >> 12), (int *)&hit);
			void *m = mmap(NULL, l, prot, MAP_SHARED, fd, (off_t)((u64)hnew << 12));
			if (m == MAP_FAILED) { printf("mmap off %llx len %llu failed errno %d\n", (unsigned long long)off, (unsigned long long)l, errno); bad++; continue; }
			if (nreg < MAXREG) { reg[nreg].off = off; reg[nreg].len = l; reg[nreg].base = m; reg[nreg].writable = !!(prot & PROT_WRITE); nreg++; }
			if (verbose) printf("mmap h=%x len=%llu prot=%u -> %p\n", hnew, (unsigned long long)l, prot, m);
		} else if (type == 3) {
			u32 k, n; memcpy(&k, q, 4); memcpy(&n, q + 4, 4); q += 8;
			int done = 0, skip = 0;
			if ((int)k <= maxkick) for (u32 i = 0; i < n; i++, q += 16 + 4096) {
				u64 ro; u32 pg; memcpy(&ro, q, 8); memcpy(&pg, q + 8, 4);
				int r = find_reg(ro);
				if (r < 0 || !reg[r].writable || (u64)(pg + 1) * 4096 > reg[r].len) { skip++; continue; }
				if (safe_cp((void *)(reg[r].base + pg * 1024), q + 16, 4096)) { skip++; continue; }
				done++;
			}
			if ((int)k <= maxkick) printf("pre-kick #%u: restored %d CPU-written pages (%d not mapped/writable)\n", k, done, skip);
		} else if (type == 4) {
			u32 tag, nr, np; memcpy(&tag, q, 4); memcpy(&nr, q + 4, 4); memcpy(&np, q + 8, 4); q += 12 + 16 * nr;
			if ((int)tag != kicks_done) continue;
			{ const char *e = getenv("MFW_SETTLE_MS"); if (e) { printf("settling %s ms before compare\n", e); fflush(stdout); usleep((useconds_t)atoi(e) * 1000); } }
			int cmp = 0, bad_pg = 0, unread = 0;
			for (int r = 0; r < nreg; r++) {
				u64 pages = reg[r].len / 4096;
				for (u64 pg = 0; pg < pages; pg++) {
					const u32 *exp = NULL; const u8 *t = q;
					for (u32 i = 0; i < np; i++, t += 16 + 4096) {
						u64 ro; u32 pi; memcpy(&ro, t, 8); memcpy(&pi, t + 8, 4);
						if (ro == reg[r].off && pi == pg) { exp = (const u32 *)(t + 16); break; }
					}
					u32 got[1024]; int mism = 0, first = -1;
					if (safe_cp(got, (const void *)(reg[r].base + pg * 1024), 4096)) { unread++; continue; }
					for (int w = 0; w < 1024; w++) {
						u32 e = exp ? exp[w] : 0, g = got[w];
						if (e != g) { mism++; if (first < 0) first = w; }
					}
					cmp++;
					if (mism) { bad_pg++; if (bad_pg <= 16) printf("after kick %u: region off %llx page %llu: %d words differ, first +%d exp %08x got %08x\n",
						tag, (unsigned long long)reg[r].off, (unsigned long long)pg, mism, first * 4, exp ? exp[first] : 0, got[first]); }
				}
			}
			printf("COMPARE after kick %u: %d pages checked, %d differ, %d unreadable (sparse holes)\n", tag, cmp, bad_pg, unread);
		}
	}
	if (stopped) {   /* still map the render target (a late mmap record) so the pixels so far can be read */
		u8 *r = p;
		while (r + 8 <= end) {
			u32 t, l; memcpy(&t, r, 4); memcpy(&l, r + 4, 4);
			if (t == 2) {
				u64 off, len; u32 prot; memcpy(&off, r + 8, 8); memcpy(&len, r + 16, 8); memcpy(&prot, r + 24, 4);
				void *m = mmap(NULL, len, prot, MAP_SHARED, fd, (off_t)off);
				if (m != MAP_FAILED && nreg < MAXREG && len == 16384) { reg[nreg].off = off; reg[nreg].len = len; reg[nreg].base = m; nreg++; }
			}
			r += 8 + l;
		}
	}
	if (kicks_done && nreg && reg[nreg - 1].len == 16384) {   /* the render target, mapped by the UM only for glReadPixels */
		u32 px[4096];
		if (safe_cp(px, (const void *)reg[nreg - 1].base, 16384)) puts("RT: unreadable");
		else {
			int nz = 0, full = 0; u32 top[4] = { 0 }; int topn[4] = { 0 };
			for (int i = 0; i < 4096; i++) {
				if (px[i]) nz++;
				if (px[i] == 0xff4080ffu) full++;
				int k, free_k = -1;
				for (k = 0; k < 4; k++) { if (topn[k] && top[k] == px[i]) { topn[k]++; break; } if (!topn[k] && free_k < 0) free_k = k; }
				if (k == 4 && free_k >= 0) { top[free_k] = px[i]; topn[free_k] = 1; }
			}
			printf("RT 64x64: %d non-zero pixels, %d == ff4080ff; pixel(32,20)=%08x pixel(2,62)=%08x\n", nz, full, px[20 * 64 + 32], px[62 * 64 + 2]);
			for (int k = 0; k < 4; k++) if (topn[k]) printf("  value %08x x%d\n", top[k], topn[k]);
		}
	}
	printf("REPLAY_DONE ioctls=%d errors=%d outputs_differing=%d kicks=%d%s\n", ioctls, bad, diffs, kicks_done, stopped ? " (stopped before next kick)" : "");
	return 0;
}
