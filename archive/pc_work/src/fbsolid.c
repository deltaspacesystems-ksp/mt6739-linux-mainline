// fbsolid: wypelnia caly ekran jednym kolorem zadanym bajtami pamieci b0 b1 b2 b3 (hex), albo rysuje linie testowe.
// uzycie: fbsolid solid  B0 B1 B2 B3          -> caly ekran: kazdy piksel = bajty B0 B1 B2 B3 (kolejnosc w pamieci)
//         fbsolid lines  STRIDE_BYTES         -> czarne tlo, 1-pikselowa biala pionowa linia x=360 i pozioma y=720
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
int main(int argc, char **argv) {
    int fd = open("/dev/fb0", O_RDWR);
    struct fb_fix_screeninfo f; struct fb_var_screeninfo v;
    if (fd < 0 || ioctl(fd, FBIOGET_FSCREENINFO, &f) || ioctl(fd, FBIOGET_VSCREENINFO, &v)) { perror("fb"); return 1; }
    uint8_t *fb = mmap(NULL, f.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb == MAP_FAILED) { perror("mmap"); return 1; }
    size_t page = (size_t)f.line_length * H;
    if (argc >= 6 && !strcmp(argv[1], "solid")) {
        uint8_t px[4] = {strtoul(argv[2],0,16), strtoul(argv[3],0,16), strtoul(argv[4],0,16), strtoul(argv[5],0,16)};
        for (size_t o = 0; o + 4 <= f.smem_len; o += 4) memcpy(fb + o, px, 4);          // caly bufor wirtualny, wszystkie strony
        printf("solid bytes %02x %02x %02x %02x w calym buforze (%u B)\n", px[0], px[1], px[2], px[3], f.smem_len);
    } else if (argc >= 3 && !strcmp(argv[1], "lines")) {
        unsigned stride = (unsigned)atoi(argv[2]);
        for (int p = 0; p < 3; p++) for (int y = 0; y < H; y++) {
            uint32_t *row = (uint32_t *)(fb + p * page + (size_t)y * stride);
            for (int x = 0; x < W; x++) row[x] = (x == 360 || y == 720) ? 0xFFFFFFFFu : 0xFF000000u;
        }
        printf("lines: stride=%u, biala pionowa x=360 i pozioma y=720 na czarnym\n", stride);
    } else { fprintf(stderr, "uzycie: solid B0 B1 B2 B3 | lines STRIDE\n"); return 2; }
    v.xoffset = 0; v.yoffset = 0; v.activate = FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE; ioctl(fd, FBIOPAN_DISPLAY, &v);
    munmap(fb, f.smem_len); close(fd); return 0;
}
