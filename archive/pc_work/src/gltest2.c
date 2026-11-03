// Test PowerVR GE8100 przez dlopen sterownika producenta (libEGL_mtk.so / libGLESv2_mtk.so), z pominieciem AOSP libEGL.so (configstore/HIDL).
typedef void *EGLDisplay, *EGLContext, *EGLSurface, *EGLConfig;
typedef unsigned int EGLBoolean, GLenum, GLbitfield;
typedef int EGLint, GLint;
typedef unsigned char GLubyte;

extern void *dlopen(const char *path, int flags);
extern void *dlsym(void *h, const char *name);
extern char *dlerror(void);
extern int write(int fd, const void *buf, unsigned long count);
extern unsigned long strlen(const char *s);
extern void _exit(int code);

#define RTLD_NOW 2
#define RTLD_GLOBAL 0x100
#define EGL_NONE 0x3038
#define EGL_SURFACE_TYPE 0x3033
#define EGL_PBUFFER_BIT 0x0001
#define EGL_RENDERABLE_TYPE 0x3040
#define EGL_OPENGL_ES2_BIT 0x0004
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
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02

static void P(const char *s) { write(1, s, strlen(s)); }
static void H(unsigned v) {
    const char *h = "0123456789abcdef"; char b[9];
    for (int i = 0; i < 8; i++) b[i] = h[(v >> ((7 - i) * 4)) & 0xF];
    b[8] = 0; P("0x"); P(b);
}
static void *handles[4]; static int nh;
static void *sym(const char *n) {
    for (int i = 0; i < nh; i++) { void *p = dlsym(handles[i], n); if (p) return p; }
    P("BRAK SYMBOLU: "); P(n); P("\n"); _exit(10); return 0;
}
static void *load(const char *path) {
    void *h = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    P("dlopen "); P(path); P(h ? " -> OK\n" : " -> BLAD: "); if (!h) { char *e = dlerror(); P(e ? e : "?"); P("\n"); }
    if (h) handles[nh++] = h;
    return h;
}

int main(void) {
    P("== gltest2: dlopen sterownika PowerVR ==\n");
    load("/vendor/lib64/egl/libEGL_mtk.so");
    load("/vendor/lib64/egl/libGLESv2_mtk.so");
    load("/vendor/lib64/libIMGegl.so");
    if (!nh) _exit(2);

    EGLDisplay (*eglGetDisplay)(void *) = sym("eglGetDisplay");
    EGLBoolean (*eglInitialize)(EGLDisplay, EGLint *, EGLint *) = sym("eglInitialize");
    EGLBoolean (*eglChooseConfig)(EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *) = sym("eglChooseConfig");
    EGLSurface (*eglCreatePbufferSurface)(EGLDisplay, EGLConfig, const EGLint *) = sym("eglCreatePbufferSurface");
    EGLContext (*eglCreateContext)(EGLDisplay, EGLConfig, EGLContext, const EGLint *) = sym("eglCreateContext");
    EGLBoolean (*eglMakeCurrent)(EGLDisplay, EGLSurface, EGLSurface, EGLContext) = sym("eglMakeCurrent");
    EGLint (*eglGetError)(void) = sym("eglGetError");
    void (*glClearColor)(float, float, float, float) = sym("glClearColor");
    void (*glClear)(GLbitfield) = sym("glClear");
    void (*glReadPixels)(GLint, GLint, GLint, GLint, GLenum, GLenum, void *) = sym("glReadPixels");
    const GLubyte *(*glGetString)(GLenum) = sym("glGetString");
    P("symbole EGL/GLES: OK\n");

    EGLDisplay dpy = eglGetDisplay((void *)0);
    P("eglGetDisplay: "); P(dpy ? "ok\n" : "NULL\n");
    EGLint maj = 0, min_ = 0;
    P("wolam eglInitialize...\n");
    if (!eglInitialize(dpy, &maj, &min_)) { P("eglInitialize FAILED, eglGetError="); H((unsigned)eglGetError()); P("\n"); _exit(1); }
    P("eglInitialize OK: "); H((unsigned)maj); P("."); H((unsigned)min_); P("\n");

    EGLint ca[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                    EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
    EGLConfig cfg; EGLint n;
    if (!eglChooseConfig(dpy, ca, &cfg, 1, &n) || n < 1) { P("eglChooseConfig FAILED err="); H((unsigned)eglGetError()); P("\n"); _exit(3); }
    P("eglChooseConfig OK\n");
    EGLint pa[] = { EGL_WIDTH, 64, EGL_HEIGHT, 64, EGL_NONE };
    EGLSurface s = eglCreatePbufferSurface(dpy, cfg, pa);
    if (!s) { P("eglCreatePbufferSurface FAILED err="); H((unsigned)eglGetError()); P("\n"); _exit(4); }
    P("pbuffer OK\n");
    EGLint xa[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext c = eglCreateContext(dpy, cfg, (void *)0, xa);
    if (!c) { P("eglCreateContext FAILED err="); H((unsigned)eglGetError()); P("\n"); _exit(5); }
    if (!eglMakeCurrent(dpy, s, s, c)) { P("eglMakeCurrent FAILED err="); H((unsigned)eglGetError()); P("\n"); _exit(6); }
    P("kontekst GLES2 aktywny\n");
    const GLubyte *v = glGetString(GL_VENDOR), *r = glGetString(GL_RENDERER), *ver = glGetString(GL_VERSION);
    P("GL_VENDOR:   "); P(v ? (const char *)v : "(null)"); P("\n");
    P("GL_RENDERER: "); P(r ? (const char *)r : "(null)"); P("\n");
    P("GL_VERSION:  "); P(ver ? (const char *)ver : "(null)"); P("\n");
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
    unsigned char px[4] = {0, 0, 0, 0};
    glReadPixels(32, 32, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
    P("piksel r,g,b,a: "); H(px[0]); P(" "); H(px[1]); P(" "); H(px[2]); P(" "); H(px[3]); P("\n");
    P((px[1] > 200 && px[0] < 50 && px[2] < 50) ? "WYNIK: GPU WYRENDEROWAL ZIELONY PIKSEL - SUKCES!\n" : "WYNIK: piksel nie zgadza sie\n");
    _exit(0); return 0;
}
