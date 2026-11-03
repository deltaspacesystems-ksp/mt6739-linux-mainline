// fbtest: wypisuje parametry mtkfb i rysuje siatke testowa z podanym stride (bajty/wiersz).
// uzycie: fbtest [stride_bytes] [pages_to_fill]
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#define W 720
#define H 1440

static uint32_t px(int r, int g, int b) { return 0xFF000000u | (r << 16) | (g << 8) | b; } // BGRA w pamieci

int main(int argc, char **argv) {
    int fd = open("/dev/fb0", O_RDWR);
    if (fd < 0) { perror("open /dev/fb0"); return 1; }
    struct fb_var_screeninfo v; struct fb_fix_screeninfo f;
    if (ioctl(fd, FBIOGET_VSCREENINFO, &v) || ioctl(fd, FBIOGET_FSCREENINFO, &f)) { perror("ioctl get"); return 1; }
    printf("id=%s smem_start=0x%lx smem_len=%u line_length=%u\n", f.id, f.smem_start, f.smem_len, f.line_length);
    printf("xres=%u yres=%u xres_virtual=%u yres_virtual=%u xoffset=%u yoffset=%u bpp=%u\n",
           v.xres, v.yres, v.xres_virtual, v.yres_virtual, v.xoffset, v.yoffset, v.bits_per_pixel);
    printf("red off=%u len=%u | green off=%u len=%u | blue off=%u len=%u | transp off=%u len=%u\n",
           v.red.offset, v.red.length, v.green.offset, v.green.length, v.blue.offset, v.blue.length, v.transp.offset, v.transp.length);
    printf("rotate=%u grayscale=%u nonstd=%u activate=0x%x height_mm=%u width_mm=%u pixclock=%u\n",
           v.rotate, v.grayscale, v.nonstd, v.activate, v.height, v.width, v.pixclock);

    unsigned stride = argc > 1 ? (unsigned)atoi(argv[1]) : f.line_length;
    int pages = argc > 2 ? atoi(argv[2]) : 3;
    size_t page_bytes = (size_t)f.line_length * H;   // jak liczy sterownik: bufor k zaczyna sie w k*line_length*yres
    uint8_t *fb = mmap(NULL, f.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb == MAP_FAILED) { perror("mmap"); return 1; }
    printf("mmap ok, stride uzyty do rysowania=%u, strony=%d, page_bytes=%zu\n", stride, pages, page_bytes);

    for (int p = 0; p < pages; p++) {
        uint8_t *base = fb + p * page_bytes;
        for (int y = 0; y < H; y++) {
            uint32_t *row = (uint32_t *)(base + (size_t)y * stride);
            for (int x = 0; x < W; x++) {
                uint32_t c;
                int band = y * 6 / H;                      // 6 pasow koloru
                static const int pal[6][3] = {{255,0,0},{255,128,0},{255,255,0},{0,255,0},{0,128,255},{128,0,255}};
                c = px(pal[band][0], pal[band][1], pal[band][2]);
                if (x % 72 == 0 || y % 144 == 0) c = px(0, 0, 0);        // siatka: pion co 72 px, poziom co 144 wierszy
                if (x < 4 || x >= W - 4 || y < 4 || y >= H - 4) c = px(255, 255, 255);   // biala ramka
                if (x == y) c = px(0, 0, 0);                              // przekatna
                row[x] = c;
            }
        }
    }
    // wymus odswiezenie / pan na strone 0
    v.xoffset = 0; v.yoffset = 0; v.activate = FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;
    int rc = ioctl(fd, FBIOPAN_DISPLAY, &v);
    printf("FBIOPAN_DISPLAY(yoffset=0) -> %d\n", rc);
    munmap(fb, f.smem_len); close(fd);
    return 0;
}
