// gpupresent: klatka pulpitu (XWD z Xvfb) -> tekstura GLES (PowerVR) -> shader -> /dev/fb0.
// Dziala w chroocie /aroot (bionic) z dlopen sterownika producenta. Argumenty: [sekundy_testu] ; env nie uzywane.
typedef void *EGLDisplay, *EGLContext, *EGLSurface, *EGLConfig;
typedef unsigned int EGLBoolean, GLenum, GLbitfield, GLuint;
typedef int EGLint, GLint, GLsizei;
typedef long ssize_t_;

extern void *dlopen(const char *path, int flags);
extern void *dlsym(void *h, const char *name);
extern int write(int fd, const void *buf, unsigned long count);
extern unsigned long strlen(const char *s);
extern void _exit(int code);
extern int open(const char *p, int fl, ...);
extern void *mmap(void *a, unsigned long len, int prot, int fl, int fd, long off);
extern int ioctl(int fd, unsigned long req, ...);
extern int memcmp(const void *a, const void *b, unsigned long n);
extern void *memcpy(void *d, const void *s, unsigned long n);
extern void *memset(void *d, int c, unsigned long n);
extern int clock_gettime(int clk, void *ts);
extern int nanosleep(const void *req, void *rem);
extern int close(int fd);
extern long lseek(int fd, long off, int wh);
extern long read(int fd, void *b, unsigned long n);

#define RTLD_NOW 2
#define RTLD_GLOBAL 0x100
#define EGL_NONE 0x3038
#define EGL_SURFACE_TYPE 0x3033
#define EGL_PBUFFER_BIT 1
#define EGL_RENDERABLE_TYPE 0x3040
#define EGL_OPENGL_ES2_BIT 4
#define EGL_RED_SIZE 0x3024
#define EGL_GREEN_SIZE 0x3023
#define EGL_BLUE_SIZE 0x3022
#define EGL_ALPHA_SIZE 0x3021
#define EGL_WIDTH 0x3057
#define EGL_HEIGHT 0x3056
#define EGL_CONTEXT_CLIENT_VERSION 0x3098
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_RGBA 0x1908
#define GL_UNSIGNED_BYTE 0x1401
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_LINEAR 0x2601
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FLOAT 0x1406
#define GL_TRIANGLE_STRIP 5
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

#define W 736
#define H 1440
#define STRIDE 2944
#define FBSIZE (STRIDE * H)

static void P(const char *s) { write(1, s, strlen(s)); }
static void N(long v) { char b[24]; int i = 23; b[i] = 0; int neg = v < 0; if (neg) v = -v; if (!v) b[--i] = '0'; while (v) { b[--i] = '0' + v % 10; v /= 10; } if (neg) b[--i] = '-'; P(b + i); }
static void *handles[4]; static int nh;
static void *sym(const char *n) {
    for (int i = 0; i < nh; i++) { void *p = dlsym(handles[i], n); if (p) return p; }
    P("BRAK SYMBOLU: "); P(n); P("\n"); _exit(10); return 0;
}
static void load(const char *path) { void *h = dlopen(path, RTLD_NOW | RTLD_GLOBAL); if (h) handles[nh++] = h; else { P("dlopen fail "); P(path); P("\n"); } }
static long now_us(void) { long ts[2]; clock_gettime(1, ts); return ts[0] * 1000000L + ts[1] / 1000; }
static void sleep_us(long us) { long ts[2] = { us / 1000000, (us % 1000000) * 1000 }; nanosleep(ts, 0); }

static const char *VS = "attribute vec2 p; varying vec2 uv; void main(){ uv = vec2(p.x*0.5+0.5, p.y*0.5+0.5); gl_Position = vec4(p,0.0,1.0); }";
// BGRX (Xvfb) -> RGBA (fb); .bgra swizzle zalatwia kolejnosc kanalow
static const char *FS = "precision mediump float; varying vec2 uv; uniform sampler2D t; void main(){ vec4 c = texture2D(t, uv); gl_FragColor = vec4(c.b, c.g, c.r, 1.0); }";

static unsigned char outbuf[FBSIZE] __attribute__((aligned(64)));
static unsigned char prev[FBSIZE] __attribute__((aligned(64)));

static unsigned char *g_fb; static int g_fd; static unsigned g_pbo[2];
static int pend[2], py0[2], prows[2], cur; static unsigned char g_var[160];
static unsigned char *(*g_map)(unsigned, long, long, unsigned); static unsigned char (*g_unmap)(unsigned); static void (*g_bind)(unsigned, unsigned);
static long copy_us;
static unsigned char shadow[FBSIZE] __attribute__((aligned(64)));   // pelny aktualny obraz wyjsciowy
static int rng0[3], rng1[3], ridx, disp;
#define pbo g_pbo
static void drain(int i) {
    long t = now_us();
    g_bind(0x88EB, g_pbo[i]);
    unsigned char *m = g_map(0x88EB, (long)py0[i] * STRIDE, (long)prows[i] * STRIDE, 1);
    if (m) {
        memcpy(shadow + (long)py0[i] * STRIDE, m, (long)prows[i] * STRIDE); g_unmap(0x88EB);
        rng0[ridx] = py0[i]; rng1[ridx] = py0[i] + prows[i] - 1; ridx = (ridx + 1) % 3;
        int u0 = 1 << 30, u1 = -1;
        for (int k = 0; k < 3; k++) if (rng1[k] >= 0) { if (rng0[k] < u0) u0 = rng0[k]; if (rng1[k] > u1) u1 = rng1[k]; }
        int back = (disp + 1) % 3;
        memcpy(g_fb + (long)back * FBSIZE + (long)u0 * STRIDE, shadow + (long)u0 * STRIDE, (long)(u1 - u0 + 1) * STRIDE);
        ((unsigned *)(g_var + 16))[1] = back * H;
        ioctl(g_fd, 0x4606, g_var); disp = back;
    }
    g_bind(0x88EB, 0);
    pend[i] = 0; copy_us += now_us() - t;
}

int main(void) {
    long secs = 0;
    P("== gpupresent ==\n");
    load("/vendor/lib64/egl/libEGL_mtk.so"); load("/vendor/lib64/egl/libGLESv2_mtk.so"); load("/vendor/lib64/libIMGegl.so");
    EGLDisplay (*eglGetDisplay)(void *) = sym("eglGetDisplay");
    EGLBoolean (*eglInitialize)(EGLDisplay, EGLint *, EGLint *) = sym("eglInitialize");
    EGLBoolean (*eglChooseConfig)(EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *) = sym("eglChooseConfig");
    EGLSurface (*eglCreatePbufferSurface)(EGLDisplay, EGLConfig, const EGLint *) = sym("eglCreatePbufferSurface");
    EGLContext (*eglCreateContext)(EGLDisplay, EGLConfig, EGLContext, const EGLint *) = sym("eglCreateContext");
    EGLBoolean (*eglMakeCurrent)(EGLDisplay, EGLSurface, EGLSurface, EGLContext) = sym("eglMakeCurrent");
    GLuint (*glCreateShader)(GLenum) = sym("glCreateShader");
    void (*glShaderSource)(GLuint, GLsizei, const char *const *, const GLint *) = sym("glShaderSource");
    void (*glCompileShader)(GLuint) = sym("glCompileShader");
    void (*glGetShaderiv)(GLuint, GLenum, GLint *) = sym("glGetShaderiv");
    GLuint (*glCreateProgram)(void) = sym("glCreateProgram");
    void (*glAttachShader)(GLuint, GLuint) = sym("glAttachShader");
    void (*glLinkProgram)(GLuint) = sym("glLinkProgram");
    void (*glGetProgramiv)(GLuint, GLenum, GLint *) = sym("glGetProgramiv");
    void (*glUseProgram)(GLuint) = sym("glUseProgram");
    GLint (*glGetAttribLocation)(GLuint, const char *) = sym("glGetAttribLocation");
    GLint (*glGetUniformLocation)(GLuint, const char *) = sym("glGetUniformLocation");
    void (*glUniform1i)(GLint, GLint) = sym("glUniform1i");
    void (*glVertexAttribPointer)(GLuint, GLint, GLenum, unsigned char, GLsizei, const void *) = sym("glVertexAttribPointer");
    void (*glEnableVertexAttribArray)(GLuint) = sym("glEnableVertexAttribArray");
    void (*glDrawArrays)(GLenum, GLint, GLsizei) = sym("glDrawArrays");
    void (*glGenTextures)(GLsizei, GLuint *) = sym("glGenTextures");
    void (*glBindTexture)(GLenum, GLuint) = sym("glBindTexture");
    void (*glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *) = sym("glTexImage2D");
    void (*glTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *) = sym("glTexSubImage2D");
    void (*glTexParameteri)(GLenum, GLenum, GLint) = sym("glTexParameteri");
    void (*glViewport)(GLint, GLint, GLsizei, GLsizei) = sym("glViewport");
    void (*glScissor)(GLint, GLint, GLsizei, GLsizei) = sym("glScissor");
    void (*glEnable)(GLenum) = sym("glEnable");
    void (*glPixelStorei)(GLenum, GLint) = sym("glPixelStorei");
    void (*glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *) = sym("glReadPixels");
    void (*glGenBuffers)(GLsizei, GLuint *) = sym("glGenBuffers");
    void (*glBindBuffer)(GLenum, GLuint) = sym("glBindBuffer");
    void (*glBufferData)(GLenum, long, const void *, GLenum) = sym("glBufferData");
    void *(*glMapBufferRange)(GLenum, long, long, GLbitfield) = sym("glMapBufferRange");
    unsigned char (*glUnmapBuffer)(GLenum) = sym("glUnmapBuffer");

    EGLDisplay dpy = eglGetDisplay((void *)0); EGLint ma, mi;
    if (!eglInitialize(dpy, &ma, &mi)) { P("eglInitialize fail\n"); _exit(1); }
    EGLint ca[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
    EGLConfig cfg; EGLint n;
    if (!eglChooseConfig(dpy, ca, &cfg, 1, &n) || n < 1) { P("choose fail\n"); _exit(2); }
    EGLint pa[] = { EGL_WIDTH, W, EGL_HEIGHT, H, EGL_NONE };
    EGLSurface s = eglCreatePbufferSurface(dpy, cfg, pa); if (!s) { P("pbuffer fail\n"); _exit(3); }
    EGLint xa[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    EGLContext c = eglCreateContext(dpy, cfg, (void *)0, xa); if (!c) { P("ctx fail\n"); _exit(4); }
    if (!eglMakeCurrent(dpy, s, s, c)) { P("makecurrent fail\n"); _exit(5); }
    P("GL ok\n");

    GLuint vs = glCreateShader(GL_VERTEX_SHADER), fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, &VS, 0); glCompileShader(vs); GLint ok = 0; glGetShaderiv(vs, GL_COMPILE_STATUS, &ok); if (!ok) { P("vs compile fail\n"); _exit(6); }
    glShaderSource(fs, 1, &FS, 0); glCompileShader(fs); glGetShaderiv(fs, GL_COMPILE_STATUS, &ok); if (!ok) { P("fs compile fail\n"); _exit(7); }
    GLuint prog = glCreateProgram(); glAttachShader(prog, vs); glAttachShader(prog, fs); glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &ok); if (!ok) { P("link fail\n"); _exit(8); }
    glUseProgram(prog);
    GLint ap = glGetAttribLocation(prog, "p"); glUniform1i(glGetUniformLocation(prog, "t"), 0);
    static const float quad[8] = { -1, -1, 1, -1, -1, 1, 1, 1 };
    glVertexAttribPointer(ap, 2, GL_FLOAT, 0, 0, quad); glEnableVertexAttribArray(ap);
    GLuint tex; glGenTextures(1, &tex); glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glViewport(0, 0, W, H); glEnable(0x0C11);

    // zrodlo: XWD z Xvfb (zbindowane do /xfb), ekran: /dev/fb0
    int xf = -1; for (int i = 0; i < 60 && xf < 0; i++) { xf = open("/xfb/Xvfb_screen0", 0); if (xf < 0) sleep_us(500000); }
    if (xf < 0) { P("brak /xfb/Xvfb_screen0\n"); _exit(11); }
    unsigned char hdr[100]; read(xf, hdr, 100);
    unsigned hs = (hdr[0] << 24) | (hdr[1] << 16) | (hdr[2] << 8) | hdr[3];
    unsigned nc = (hdr[76] << 24) | (hdr[77] << 16) | (hdr[78] << 8) | hdr[79];
    long off = hs + nc * 12L;
    unsigned char *xm = mmap(0, off + FBSIZE, 1, 1, xf, 0); if ((long)xm == -1) { P("mmap xfb fail\n"); _exit(12); }
    unsigned char *src = xm + off;
    int fd = open("/dev/fb0", 2); if (fd < 0) { P("open fb0 fail\n"); _exit(13); }
    unsigned char *fb = mmap(0, FBSIZE * 3L, 3, 1, fd, 0); if ((long)fb == -1) { P("mmap fb fail\n"); _exit(14); }
    unsigned char var[160]; ioctl(fd, 0x4600, var);
    ((unsigned *)(var + 16))[0] = 0; ((unsigned *)(var + 16))[1] = 0; *(unsigned *)(var + 84) = 0x80;
    g_fb = fb; g_fd = fd; for (int i = 0; i < 160; i++) g_var[i] = var[i];
    g_map = (unsigned char *(*)(unsigned, long, long, unsigned))glMapBufferRange; g_unmap = (unsigned char (*)(unsigned))glUnmapBuffer; g_bind = (void (*)(unsigned, unsigned))glBindBuffer;
    glGenBuffers(2, g_pbo);
    for (int i = 0; i < 2; i++) { glBindBuffer(0x88EB, g_pbo[i]); glBufferData(0x88EB, FBSIZE, 0, 0x88E1); }
    glBindBuffer(0x88EB, 0);
    for (int i = 0; i < 3; i++) { rng0[i] = 1 << 30; rng1[i] = -1; }
    P("start petli (PBO + 3 strony)\n");

    long t0 = now_us(), tl = t0, frames = 0, tex_us = 0, draw_us = 0;
    memset(prev, 0, 16);
    int first = 1;
    for (;;) {
        long a = now_us();
        int y0 = -1, y1 = -1;
        if (first) { y0 = 0; y1 = H - 1; }
        else for (int y = 0; y < H; y++) if (memcmp(src + y * STRIDE, prev + y * STRIDE, STRIDE)) { if (y0 < 0) y0 = y; y1 = y; }
        long tt = 0, td = 0, tc = 0;
        if (y0 >= 0) {
            first = 0;
            int rows = y1 - y0 + 1;
            long a1 = now_us();
            memcpy(prev + y0 * STRIDE, src + y0 * STRIDE, (long)rows * STRIDE);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, y0, W, rows, GL_RGBA, GL_UNSIGNED_BYTE, prev + y0 * STRIDE);
            long b = now_us();
            glScissor(0, y0, W, rows);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindBuffer(0x88EB, pbo[cur]);
            glReadPixels(0, y0, W, rows, GL_RGBA, GL_UNSIGNED_BYTE, (void *)(long)(y0 * STRIDE));
            glBindBuffer(0x88EB, 0);
            long d = now_us();
            tt = b - a1; td = d - b;
            int prevb = cur ^ 1; int hadpend = pend[prevb];
            pend[cur] = 1; py0[cur] = y0; prows[cur] = rows; cur ^= 1;
            if (hadpend) { drain(prevb); }
            frames++;
        } else if (pend[cur ^ 1]) { drain(cur ^ 1); }
        tex_us += tt; draw_us += td;
        long t = now_us();
        if (t - tl > 2000000) {
            P("klatki: "); N(frames); P(" w "); N((t - t0) / 1000); P(" ms | avg tex/draw+read/copy us: ");
            if (frames) { N(tex_us / frames); P("/"); N(draw_us / frames); P("/"); N(copy_us / frames); } P("\n"); tl = t;
        }
        if (secs && (t - t0) > secs * 1000000L) break;
        long used = now_us() - a; if (used < 16000) sleep_us(16000 - used);
    }
    P("koniec\n");
    _exit(0); return 0;
}
