/* Test wydajnosci rysowania programowego (pixman, NEON) na MT6739 przy 736x1440.
 * Mierzy: kopie pelnego ekranu, OVER pelnoekranowy, kilka polprzezroczystych zaokraglonych paneli
 * oraz rozmycie tla pod "szklem" (downscale x4 + box blur + upscale). */
#define _POSIX_C_SOURCE 200809L
#include <pixman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define W 736
#define H 1440

static double now_ms(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}

static pixman_image_t *mk(int w, int h, uint32_t fill, uint32_t **out)
{
    uint32_t *p = malloc((size_t)w * h * 4);
    for (size_t i = 0; i < (size_t)w * h; i++) p[i] = fill;
    if (out) *out = p;
    return pixman_image_create_bits(PIXMAN_a8r8g8b8, w, h, p, w * 4);
}

/* rozmycie pudelkowe 1 przebieg poziomy+pionowy na obrazie ARGB (promien r), oddzielne kanaly */
static void box_blur(uint32_t *img, uint32_t *tmp, int w, int h, int r)
{
    int n = 2 * r + 1;
    for (int y = 0; y < h; y++) {
        uint32_t *row = img + (size_t)y * w, *o = tmp + (size_t)y * w;
        unsigned a = 0, rr = 0, g = 0, b = 0;
        for (int x = -r; x <= r; x++) {
            uint32_t c = row[x < 0 ? 0 : (x >= w ? w - 1 : x)];
            a += c >> 24; rr += (c >> 16) & 255; g += (c >> 8) & 255; b += c & 255;
        }
        for (int x = 0; x < w; x++) {
            o[x] = ((a / n) << 24) | ((rr / n) << 16) | ((g / n) << 8) | (b / n);
            int xi = x + r + 1, xo = x - r;
            uint32_t ci = row[xi >= w ? w - 1 : xi], co = row[xo < 0 ? 0 : xo];
            a += (ci >> 24) - (co >> 24);
            rr += ((ci >> 16) & 255) - ((co >> 16) & 255);
            g += ((ci >> 8) & 255) - ((co >> 8) & 255);
            b += (ci & 255) - (co & 255);
        }
    }
    for (int x = 0; x < w; x++) {
        unsigned a = 0, rr = 0, g = 0, b = 0;
        for (int y = -r; y <= r; y++) {
            uint32_t c = tmp[(size_t)(y < 0 ? 0 : (y >= h ? h - 1 : y)) * w + x];
            a += c >> 24; rr += (c >> 16) & 255; g += (c >> 8) & 255; b += c & 255;
        }
        for (int y = 0; y < h; y++) {
            img[(size_t)y * w + x] = ((a / n) << 24) | ((rr / n) << 16) | ((g / n) << 8) | (b / n);
            int yi = y + r + 1, yo = y - r;
            uint32_t ci = tmp[(size_t)(yi >= h ? h - 1 : yi) * w + x], co = tmp[(size_t)(yo < 0 ? 0 : yo) * w + x];
            a += (ci >> 24) - (co >> 24);
            rr += ((ci >> 16) & 255) - ((co >> 16) & 255);
            g += ((ci >> 8) & 255) - ((co >> 8) & 255);
            b += (ci & 255) - (co & 255);
        }
    }
}

#define RUN(label, iters, stmt)                                                    \
    do {                                                                           \
        double t0 = now_ms();                                                      \
        for (int _i = 0; _i < (iters); _i++) { stmt; }                             \
        double dt = (now_ms() - t0) / (iters);                                     \
        printf("%-52s %8.2f ms  (%6.1f fps max)\n", label, dt, 1000.0 / dt);       \
    } while (0)

int main(void)
{
    uint32_t *dp, *sp, *pp;
    pixman_image_t *dst = mk(W, H, 0xff203040, &dp);
    pixman_image_t *src_op = mk(W, H, 0xff40a060, &sp);   /* nieprzezroczysta tapeta */
    pixman_image_t *src_tr = mk(W, H, 0x80102030, &pp);   /* polprzezroczysta warstwa */
    printf("== pixman %s, %dx%d ==\n", pixman_version_string(), W, H);

    RUN("memcpy pelny ekran (4.2 MB)", 60, memcpy(dp, sp, (size_t)W * H * 4));
    RUN("SRC pelny ekran (tapeta)", 60,
        pixman_image_composite32(PIXMAN_OP_SRC, src_op, NULL, dst, 0, 0, 0, 0, 0, 0, W, H));
    RUN("OVER pelny ekran, warstwa polprzezroczysta", 60,
        pixman_image_composite32(PIXMAN_OP_OVER, src_tr, NULL, dst, 0, 0, 0, 0, 0, 0, W, H));

    /* centrum sterowania: 4 panele 230x180 polprzezroczyste */
    RUN("OVER 4 panele 230x180 (typowe centrum sterowania)", 200, {
        for (int k = 0; k < 4; k++)
            pixman_image_composite32(PIXMAN_OP_OVER, src_tr, NULL, dst, 0, 0, 0, 0, 20 + k * 40, 100 + k * 20, 230, 180);
    });

    /* karta przelacznika: skalowanie 720x1440 -> 360x720 (transformacja) */
    pixman_transform_t tr;
    pixman_transform_init_scale(&tr, pixman_double_to_fixed(2.0), pixman_double_to_fixed(2.0));
    pixman_image_set_transform(src_op, &tr);
    pixman_image_set_filter(src_op, PIXMAN_FILTER_BILINEAR, NULL, 0);
    RUN("zmniejszona karta 0.5x (bilinear, 360x720)", 60,
        pixman_image_composite32(PIXMAN_OP_OVER, src_op, NULL, dst, 0, 0, 0, 0, 100, 200, 360, 720));
    pixman_transform_init_scale(&tr, pixman_double_to_fixed(1.0), pixman_double_to_fixed(1.0));
    pixman_image_set_transform(src_op, &tr);

    /* szklo: rozmycie tla pod panelem 360x300 (downscale x4 -> 90x75, blur r=6, upscale) */
    {
        int bw = 90, bh = 75;
        uint32_t *b1 = malloc((size_t)bw * bh * 4), *b2 = malloc((size_t)bw * bh * 4);
        memset(b1, 0x55, (size_t)bw * bh * 4);
        RUN("blur szkla: panel 360x300 (1/4 + box r=6)", 100, box_blur(b1, b2, bw, bh, 6));
        int fw = 184, fh = 360;
        uint32_t *c1 = malloc((size_t)fw * fh * 4), *c2 = malloc((size_t)fw * fh * 4);
        memset(c1, 0x55, (size_t)fw * fh * 4);
        RUN("blur szkla: caly ekran 1/4 (184x360, box r=6)", 30, box_blur(c1, c2, fw, fh, 6));
    }

    (void)pp;
    return 0;
}
