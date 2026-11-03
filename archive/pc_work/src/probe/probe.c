// Bare-metal probe jako "kernel" arm64: LK ladujе go pod 0x40000000 i skacze; rysujemy rejestry na framebufferze.
// Kolejnosc wierszy (od gory), kazdy: kolorowy znacznik + 16 cyfr hex:
//  1 x0 (wskaznik DTB od LK)   2 x1   3 x2   4 x3
//  5 CurrentEL (EL=wartosc>>2) 6 MIDR_EL1  7 MPIDR_EL1  8 SCTLR (EL1 lub EL2: patrz wiersz 5)
//  9 CNTFRQ_EL0               10 PC (adres _start)  11 slowo pod *x0 (magic DTB: powinno byc EDFE0DD0)
// 12 totalsize DTB (big-endian -> hex)   13 pierwsze 8 bajtow bootargs-owego chosen? (pomijamy)
typedef unsigned long u64; typedef unsigned int u32; typedef unsigned char u8;
#define FB   ((volatile u32 *)0x771e0000UL)
#define STRIDE_PX 736
#define W 720
#define H 1440

static const u8 G[16][5] = {{7,5,5,5,7},{2,6,2,2,7},{7,1,7,4,7},{7,1,7,1,7},{5,5,7,1,1},{7,4,7,1,7},{7,4,7,5,7},{7,1,1,1,1},
                            {7,5,7,5,7},{7,5,7,1,7},{7,5,7,5,5},{6,5,6,5,6},{7,4,4,4,7},{6,5,5,5,6},{7,4,7,4,7},{7,4,7,4,4}};
#define PX(r,g,b) ((u32)(r) | ((u32)(g) << 8) | ((u32)(b) << 16) | 0xFF000000u)

static void rect(int x, int y, int w, int h, u32 c) {
    for (int j = 0; j < h; j++) for (int i = 0; i < w; i++) {
        int xx = x + i, yy = y + j; if (xx < 0 || yy < 0 || xx >= W || yy >= H) continue;
        FB[yy * STRIDE_PX + xx] = c;
    }
}
static void digit(int x, int y, int d, u32 c) {
    for (int r = 0; r < 5; r++) for (int b = 0; b < 3; b++)
        if (G[d][r] & (4 >> b)) rect(x + b * 6, y + r * 6, 6, 6, c);
}
static void row(int idx, u32 marker, u64 v) {
    int y = 40 + idx * 52;
    rect(8, y, 24, 30, marker);
    for (int n = 0; n < 16; n++) digit(44 + n * 41, y, (int)((v >> (60 - 4 * n)) & 0xF), PX(255, 255, 255));
}
static u32 bswap32(u32 v) { return (v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | (v << 24); }

u64 g_x[4];
void probe_main(void) {
    u64 el, midr, mpidr, sctlr, cntfrq, pc;
    __asm__ volatile("mrs %0, CurrentEL" : "=r"(el));
    __asm__ volatile("mrs %0, MIDR_EL1" : "=r"(midr));
    __asm__ volatile("mrs %0, MPIDR_EL1" : "=r"(mpidr));
    if (((el >> 2) & 3) == 2) __asm__ volatile("mrs %0, SCTLR_EL2" : "=r"(sctlr)); else __asm__ volatile("mrs %0, SCTLR_EL1" : "=r"(sctlr));
    __asm__ volatile("mrs %0, CNTFRQ_EL0" : "=r"(cntfrq));
    __asm__ volatile("adr %0, ." : "=r"(pc));
    // tlo: ciemny granat (zeby bylo widac, ze to nasz kod, a nie LK/logo)
    rect(0, 0, W, H, PX(0, 0, 40));
    u32 dtbmagic = 0, dtbsize = 0;
    if (g_x[0] >= 0x40000000UL && g_x[0] < 0xC0000000UL && (g_x[0] & 3) == 0) {
        dtbmagic = *(volatile u32 *)g_x[0]; dtbsize = bswap32(*(volatile u32 *)(g_x[0] + 4));
    }
    row(0,  PX(255,0,0),    g_x[0]);
    row(1,  PX(255,128,0),  g_x[1]);
    row(2,  PX(255,255,0),  g_x[2]);
    row(3,  PX(0,255,0),    g_x[3]);
    row(4,  PX(0,255,255),  el);
    row(5,  PX(0,128,255),  midr);
    row(6,  PX(128,0,255),  mpidr);
    row(7,  PX(255,0,255),  sctlr);
    row(8,  PX(255,255,255),cntfrq);
    row(9,  PX(160,160,160),pc);
    row(10, PX(255,128,128),dtbmagic);
    row(11, PX(128,255,128),dtbsize);
    // pasek wyniku: zielony = DTB ma poprawny magic, czerwony = nie
    rect(8, 40 + 13 * 52, W - 16, 40, dtbmagic == 0xEDFE0DD0u ? PX(0,200,0) : PX(220,0,0));
    for (;;) __asm__ volatile("wfe");
}

__attribute__((naked, section(".head"))) void _start(void) {
    __asm__ volatile(
        "b 1f\n"
        ".word 0\n"                      // code1
        ".quad 0\n"                      // text_offset
        ".quad 0x10000\n"                // image_size
        ".quad 0xa\n"                    // flags: LE, 4K, anywhere
        ".quad 0\n.quad 0\n.quad 0\n"    // res2..res4
        ".ascii \"ARM\\x64\"\n"          // magic @0x38
        ".word 0\n"                      // res5
        "1:\n"
        "adrp x8, g_x\n"
        "add  x8, x8, :lo12:g_x\n"
        "stp  x0, x1, [x8]\n"
        "stp  x2, x3, [x8, #16]\n"
        "ldr  x9, =0x42000000\n"
        "mov  sp, x9\n"
        "bl   probe_main\n"
        "2: wfe\n b 2b\n"
    );
}
