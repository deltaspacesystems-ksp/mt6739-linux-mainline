// Test prawdziwego PowerVR GE8100 przez bionic libEGL.so/libGLESv2.so (bez naglowkow systemowych - wlasne deklaracje).
// Surowe typy, zeby nie potrzebowac <EGL/egl.h> ani <GLES2/gl2.h>.
typedef void *EGLDisplay, *EGLContext, *EGLSurface, *EGLConfig;
typedef unsigned int EGLBoolean, EGLenum, GLenum, GLbitfield, GLuint;
typedef int EGLint, GLint;
typedef unsigned char GLubyte;

extern EGLDisplay eglGetDisplay(void *native_display);
extern EGLBoolean eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor);
extern EGLBoolean eglChooseConfig(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
extern EGLSurface eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
extern EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
extern EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
extern EGLint eglGetError(void);
extern const char *eglQueryString(EGLDisplay dpy, EGLint name);

extern void glClearColor(float r, float g, float b, float a);
extern void glClear(GLbitfield mask);
extern void glReadPixels(GLint x, GLint y, GLint width, GLint height, GLenum format, GLenum type, void *pixels);
extern const GLubyte *glGetString(GLenum name);
extern GLenum glGetError(void);

extern int write(int fd, const void *buf, unsigned long count);
extern unsigned long strlen(const char *s);
extern void _exit(int code);

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
static void hex4(unsigned v, char *out) {
    const char *h = "0123456789abcdef";
    for (int i = 0; i < 4; i++) out[i] = h[(v >> ((3 - i) * 4)) & 0xF];
}

int main(void) {
    EGLDisplay dpy = eglGetDisplay((void *)0);
    P("eglGetDisplay: "); P(dpy ? "ok\n" : "NULL\n");
    EGLint maj, min_;
    if (!eglInitialize(dpy, &maj, &min_)) { P("eglInitialize FAILED\n"); _exit(1); }
    P("eglInitialize OK, wersja: "); char b[6]; hex4((unsigned)maj, b); write(1, b, 4); P(".");  hex4((unsigned)min_, b); write(1, b, 4); P("\n");

    EGLint cfg_attr[] = { EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                          EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
    EGLConfig cfg; EGLint ncfg;
    if (!eglChooseConfig(dpy, cfg_attr, &cfg, 1, &ncfg) || ncfg < 1) { P("eglChooseConfig FAILED\n"); _exit(2); }
    P("eglChooseConfig OK\n");

    EGLint pb_attr[] = { EGL_WIDTH, 64, EGL_HEIGHT, 64, EGL_NONE };
    EGLSurface surf = eglCreatePbufferSurface(dpy, cfg, pb_attr);
    if (!surf) { P("eglCreatePbufferSurface FAILED\n"); _exit(3); }
    P("eglCreatePbufferSurface OK\n");

    EGLint ctx_attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext ctx = eglCreateContext(dpy, cfg, (void *)0, ctx_attr);
    if (!ctx) { P("eglCreateContext FAILED\n"); _exit(4); }
    P("eglCreateContext OK\n");

    if (!eglMakeCurrent(dpy, surf, surf, ctx)) { P("eglMakeCurrent FAILED\n"); _exit(5); }
    P("eglMakeCurrent OK - kontekst GLES2 aktywny na prawdziwym PowerVR!\n");

    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);
    P("GL_VENDOR:   "); P((const char *)(vendor ? vendor : (const GLubyte *)"(null)")); P("\n");
    P("GL_RENDERER: "); P((const char *)(renderer ? renderer : (const GLubyte *)"(null)")); P("\n");
    P("GL_VERSION:  "); P((const char *)(version ? version : (const GLubyte *)"(null)")); P("\n");

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);   // zielony
    glClear(GL_COLOR_BUFFER_BIT);
    unsigned char px[4] = {0, 0, 0, 0};
    glReadPixels(32, 32, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
    P("Piksel po glClear(zielony) r,g,b,a: ");
    for (int i = 0; i < 4; i++) { char d[2] = { (char)('0' + px[i] / 100), 0 }; char h2[8]; hex4(px[i], h2); write(1, h2, 4); P(" "); }
    P("\n");
    if (px[1] > 200 && px[0] < 50 && px[2] < 50) P("WYNIK: GPU NAPRAWDE WYRENDEROWAL ZIELONY PIKSEL - SUKCES!\n");
    else P("WYNIK: piksel nie zgadza sie - do zbadania\n");
    _exit(0);
    return 0;
}
