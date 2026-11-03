// Test: bufor gralloc (ION) jako cel renderowania GPU (EGLImage -> tekstura -> FBO), potem odczyt CPU przez lock.
typedef void *EGLDisplay, *EGLContext, *EGLSurface, *EGLConfig, *EGLImage;
typedef unsigned int EGLBoolean, GLenum, GLbitfield, GLuint;
typedef int EGLint, GLint, GLsizei;
extern void *dlopen(const char *path, int flags);
extern void *dlsym(void *h, const char *name);
extern char *dlerror(void);
extern int write(int fd, const void *buf, unsigned long count);
extern unsigned long strlen(const char *s);
extern void _exit(int code);
extern int clock_gettime(int clk, void *ts);
static void P(const char *s) { write(1, s, strlen(s)); }
static void H(unsigned v) { const char *h = "0123456789abcdef"; char b[9]; for (int i = 0; i < 8; i++) b[i] = h[(v >> ((7 - i) * 4)) & 15]; b[8] = 0; P("0x"); P(b); }
static long now_us(void) { long ts[2]; clock_gettime(1, ts); return ts[0] * 1000000L + ts[1] / 1000; }
static void *handles[4]; static int nh;
static void *sym(const char *n) { for (int i = 0; i < nh; i++) { void *p = dlsym(handles[i], n); if (p) return p; } P("BRAK SYMBOLU: "); P(n); P("\n"); _exit(10); return 0; }
static void load(const char *path) { void *h = dlopen(path, 2 | 0x100); if (h) handles[nh++] = h; else { P("dlopen fail "); P(path); P(": "); char *e = dlerror(); P(e ? e : "?"); P("\n"); } }

#define EGL_NONE 0x3038
#define W 736
#define H_ 1440

int main(void) {
    P("== grtest ==\n");
    load("/vendor/lib64/egl/libEGL_mtk.so"); load("/vendor/lib64/egl/libGLESv2_mtk.so"); load("/vendor/lib64/libIMGegl.so");
    // --- gralloc
    void *gh = dlopen("/vendor/lib64/hw/gralloc.mt6739.so", 2 | 0x100);
    if (!gh) { P("gralloc dlopen fail\n"); _exit(1); }
    char *mod = dlsym(gh, "HMI"); if (!mod) { P("no HMI\n"); _exit(2); }
    P("gralloc module id: "); P(*(char **)(mod + 8)); P("\n");
    char *meth = *(char **)(mod + 32);
    int (*op)(void *, const char *, void **) = *(void **)meth;
    char *adev = 0; int rr = op(mod, "gpu0", (void **)&adev);
    P("open gpu0 r="); H(rr); P("\n"); if (rr || !adev) _exit(3);
    int (*galloc)(void *, int, int, int, int, void **, int *) = *(void **)(adev + 72);
    void *bh = 0; int stride = 0;
    // usage: SW_READ_OFTEN(0x3) | HW_TEXTURE(0x100) | HW_RENDER(0x200)
    rr = galloc(adev, W, H_, 1, 0x303, &bh, &stride);
    P("alloc r="); H(rr); P(" stride="); H(stride); P("\n"); if (rr || !bh) _exit(4);
    P("handle numFds/numInts: "); H(((int *)bh)[1]); P(" "); H(((int *)bh)[2]); P("\n");
    // ANativeWindowBuffer
    static unsigned char nb[256] __attribute__((aligned(16)));
    *(int *)(nb + 0) = 0x5f776e62; *(int *)(nb + 4) = 176;
    static void (*dummy)(void *) = 0;
    *(int *)(nb + 56) = W; *(int *)(nb + 60) = H_; *(int *)(nb + 64) = stride; *(int *)(nb + 68) = 1; *(int *)(nb + 72) = 0x303;
    *(long *)(nb + 80) = 1;  // layerCount (API26) / reserved
    *(void **)(nb + 96) = bh; *(unsigned long *)(nb + 104) = 0x303;
    // --- EGL
    EGLDisplay (*eglGetDisplay)(void *) = sym("eglGetDisplay");
    EGLBoolean (*eglInitialize)(EGLDisplay, EGLint *, EGLint *) = sym("eglInitialize");
    EGLBoolean (*eglChooseConfig)(EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *) = sym("eglChooseConfig");
    EGLSurface (*eglCreatePbufferSurface)(EGLDisplay, EGLConfig, const EGLint *) = sym("eglCreatePbufferSurface");
    EGLContext (*eglCreateContext)(EGLDisplay, EGLConfig, EGLContext, const EGLint *) = sym("eglCreateContext");
    EGLBoolean (*eglMakeCurrent)(EGLDisplay, EGLSurface, EGLSurface, EGLContext) = sym("eglMakeCurrent");
    EGLint (*eglGetError)(void) = sym("eglGetError");
    void *(*eglGetProcAddress)(const char *) = sym("eglGetProcAddress");
    EGLDisplay dpy = eglGetDisplay(0); EGLint ma, mi;
    if (!eglInitialize(dpy, &ma, &mi)) { P("egl init fail\n"); _exit(5); }
    EGLint ca[] = { 0x3033, 1, 0x3040, 4, 0x3024, 8, 0x3023, 8, 0x3022, 8, 0x3021, 8, EGL_NONE };
    EGLConfig cfg; EGLint n; if (!eglChooseConfig(dpy, ca, &cfg, 1, &n) || n < 1) { P("choose fail\n"); _exit(6); }
    EGLint pa[] = { 0x3057, 16, 0x3056, 16, EGL_NONE };
    EGLSurface s = eglCreatePbufferSurface(dpy, cfg, pa);
    EGLint xa[] = { 0x3098, 3, EGL_NONE };
    EGLContext c = eglCreateContext(dpy, cfg, 0, xa); if (!c) { P("ctx fail\n"); _exit(7); }
    eglMakeCurrent(dpy, s, s, c);
    EGLImage (*eglCreateImageKHR)(EGLDisplay, EGLContext, EGLint, void *, const EGLint *) = eglGetProcAddress("eglCreateImageKHR");
    void (*imgTarget)(GLenum, EGLImage) = eglGetProcAddress("glEGLImageTargetTexture2DOES");
    P(eglCreateImageKHR ? "eglCreateImageKHR ok\n" : "NO eglCreateImageKHR\n"); P(imgTarget ? "glEGLImageTargetTexture2DOES ok\n" : "NO imgTarget\n");
    if (!eglCreateImageKHR || !imgTarget) _exit(8);
    EGLint ia[] = { 0x30D2 /*EGL_IMAGE_PRESERVED_KHR*/, 1, EGL_NONE };
    EGLImage img = eglCreateImageKHR(dpy, 0, 0x3140 /*EGL_NATIVE_BUFFER_ANDROID*/, nb, ia);
    P("eglCreateImageKHR -> "); H((unsigned)(long)img); P(" err="); H((unsigned)eglGetError()); P("\n"); if (!img) _exit(9);
    void (*glGenTextures)(GLsizei, GLuint *) = sym("glGenTextures");
    void (*glBindTexture)(GLenum, GLuint) = sym("glBindTexture");
    void (*glGenFramebuffers)(GLsizei, GLuint *) = sym("glGenFramebuffers");
    void (*glBindFramebuffer)(GLenum, GLuint) = sym("glBindFramebuffer");
    void (*glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint) = sym("glFramebufferTexture2D");
    GLenum (*glCheckFramebufferStatus)(GLenum) = sym("glCheckFramebufferStatus");
    void (*glClearColor)(float, float, float, float) = sym("glClearColor");
    void (*glClear)(GLbitfield) = sym("glClear");
    void (*glViewport)(GLint, GLint, GLsizei, GLsizei) = sym("glViewport");
    void (*glFinish)(void) = sym("glFinish");
    GLuint (*glGetError)(void) = sym("glGetError");
    GLuint tex, fbo; glGenTextures(1, &tex); glBindTexture(0x0DE1, tex); imgTarget(0x0DE1, img);
    P("target tex glErr="); H(glGetError()); P("\n");
    glGenFramebuffers(1, &fbo); glBindFramebuffer(0x8D40, fbo); glFramebufferTexture2D(0x8D40, 0x8CE0, 0x0DE1, tex, 0);
    P("fbo status="); H(glCheckFramebufferStatus(0x8D40)); P("\n");
    glViewport(0, 0, W, H_);
    long t0 = now_us();
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); glClear(0x4000); glFinish();
    P("clear us="); H((unsigned)(now_us() - t0)); P("\n");
    // CPU lock
    int (*lock)(void *, void *, int, int, int, int, int, void **) = *(void **)(mod + 264);
    int (*unlock)(void *, void *) = *(void **)(mod + 272);
    void *va = 0; long t1 = now_us();
    rr = lock(mod, bh, 0x3, 0, 0, W, H_, &va);
    P("lock r="); H(rr); P(" va="); H((unsigned)(long)va); P(" us="); H((unsigned)(now_us() - t1)); P("\n");
    if (va) {
        unsigned char *p = va; long off = 100L * stride * 4 + 50 * 4;
        P("px r,g,b,a: "); H(p[off]); P(" "); H(p[off + 1]); P(" "); H(p[off + 2]); P(" "); H(p[off + 3]); P("\n");
        static unsigned char dst[736 * 4 * 1440]; long t2 = now_us();
        for (long i = 0; i < 736L * 4 * 1440; i += 8) *(unsigned long *)(dst + i) = *(unsigned long *)(p + i);
        P("cpu read 4MB us="); H((unsigned)(now_us() - t2)); P("\n");
        unlock(mod, bh);
    }
    _exit(0); return 0;
}
