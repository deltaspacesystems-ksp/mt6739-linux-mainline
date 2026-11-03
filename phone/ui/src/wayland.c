#define _GNU_SOURCE
#include "ui.h"
#include "motion.h"
#include "sampler.h"
#include "gestures.h"
#include "xdg-shell-client-protocol.h"
#include "xdg-decoration-client-protocol.h"
#include <wayland-client.h>
#include <sys/mman.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <linux/input-event-codes.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct app;
struct buffer { struct wl_buffer *wl; void *data; size_t size; int w,h; bool busy; };
struct app {
    struct wl_display *display; struct wl_compositor *compositor; struct wl_shm *shm;
    struct xdg_wm_base *wm; struct wl_surface *surface;
    struct xdg_surface *xdg; struct xdg_toplevel *top;
    struct zxdg_decoration_manager_v1 *decoration_manager;
    struct zxdg_toplevel_decoration_v1 *decoration;
    struct wl_seat *seat; struct wl_pointer *pointer; struct wl_touch *touch;
    struct wl_callback *frame; struct buffer buffers[3];
    struct mfw_motion motion; struct mfw_model *model; int width,height,frames,limit;
    bool configured,dirty,running,windowed,maximized,down;
    double x,y;
    struct mfw_gestures gestures;
};
static volatile sig_atomic_t stopped;
static void stop(int sig) { (void)sig; stopped=1; }
static void buffer_free(struct buffer *b) {
    if(b->wl)wl_buffer_destroy(b->wl);
    if(b->data)munmap(b->data,b->size);
    memset(b,0,sizeof *b);
}
static void release(void *data,struct wl_buffer *wl) { (void)wl; ((struct buffer *)data)->busy=false; }
static const struct wl_buffer_listener buffer_listener={.release=release};
static bool buffer_make(struct app *a,struct buffer *b) {
    buffer_free(b);
    if(a->width<1||a->height<1||a->width>4096||a->height>4096)return false;
    b->size=(size_t)a->width*a->height*4;
    int fd=memfd_create("microfw-ui",MFD_CLOEXEC);
    if(fd<0)return false;
    if(ftruncate(fd,(off_t)b->size)<0) {close(fd);return false;}
    b->data=mmap(NULL,b->size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(b->data==MAP_FAILED) {b->data=NULL;close(fd);return false;}
    struct wl_shm_pool *pool=wl_shm_create_pool(a->shm,fd,(int)b->size);
    b->wl=wl_shm_pool_create_buffer(pool,0,a->width,a->height,a->width*4,WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);close(fd);b->w=a->width;b->h=a->height;
    if(!b->wl){buffer_free(b);return false;}
    wl_buffer_add_listener(b->wl,&buffer_listener,b);return true;
}
static void frame_done(void *data,struct wl_callback *cb,uint32_t ms) {
    (void)ms; struct app *a=data;wl_callback_destroy(cb);a->frame=NULL;
}
static const struct wl_callback_listener frame_listener={.done=frame_done};
static void draw(struct app *a) {
    if(!a->configured||a->frame||!a->dirty||(a->limit&&a->frames>=a->limit))return;
    struct buffer *b=NULL;
    for(int i=0;i<3;i++) if(!a->buffers[i].busy){b=&a->buffers[i];break;}
    if(!b)return;
    if(!b->wl||b->w!=a->width||b->h!=a->height) {
        if(!buffer_make(a,b)){fprintf(stderr,"Cannot allocate Wayland buffer\n");a->running=false;return;}
    }
    cairo_surface_t *s=cairo_image_surface_create_for_data(b->data,CAIRO_FORMAT_RGB24,b->w,b->h,b->w*4);
    cairo_t *c=cairo_create(s);bool animating=mfw_motion_render(&a->motion,c,b->w,b->h,a->model,mfw_motion_now());
    cairo_destroy(c);cairo_surface_flush(s);cairo_surface_destroy(s);
    a->frame=wl_surface_frame(a->surface);wl_callback_add_listener(a->frame,&frame_listener,a);
    b->busy=true;wl_surface_attach(a->surface,b->wl,0,0);
    wl_surface_damage(a->surface,0,0,b->w,b->h);wl_surface_commit(a->surface);
    a->dirty=animating;a->frames++;
    fprintf(stderr,"MICROFW frame=%d page=%d size=%dx%d\n",a->frames,a->model->page,b->w,b->h);
}
static void configure(void *data,struct xdg_surface *s,uint32_t serial) {
    struct app *a=data;xdg_surface_ack_configure(s,serial);a->configured=true;a->dirty=true;
}
static const struct xdg_surface_listener surface_listener={.configure=configure};
static void top_configure(void *data,struct xdg_toplevel *t,int32_t w,int32_t h,struct wl_array *states) {
    (void)t;(void)states;struct app *a=data;
    if(w>0&&h>0&&w<=4096&&h<=4096){
        if(w!=a->width||h!=a->height)mfw_gestures_cancel(&a->gestures);
        a->width=w;a->height=h;
    } a->dirty=true;
}
static void top_close(void *data,struct xdg_toplevel *t) { (void)t;((struct app *)data)->running=false; }
static const struct xdg_toplevel_listener top_listener={.configure=top_configure,.close=top_close};
static void decoration_configure(void*d,struct zxdg_toplevel_decoration_v1*dec,uint32_t mode){(void)d;(void)dec;(void)mode;}
static const struct zxdg_toplevel_decoration_v1_listener decoration_listener={.configure=decoration_configure};
static void ping(void *data,struct xdg_wm_base *wm,uint32_t serial) { (void)data;xdg_wm_base_pong(wm,serial); }
static const struct xdg_wm_base_listener wm_listener={.ping=ping};
static void gesture(struct app *a,struct mfw_gesture_result result) {
    double scale=1.0/a->model->scale, h=a->height*scale,w=a->width*scale;
    double x=result.x,y=result.y;
    if(result.action==ACTION_DESKTOP)a->model->page=PAGE_DESKTOP;
    else if(result.action==ACTION_OVERVIEW)a->model->page=PAGE_SWITCHER;
    else if(result.action==ACTION_CONTROL)a->model->page=PAGE_CONTROL;
    else if(result.action==ACTION_LEAVE_FULLSCREEN){xdg_toplevel_unset_fullscreen(a->top);a->windowed=true;}
    else if(result.action==ACTION_TAP) {
        if(a->model->page==PAGE_DESKTOP&&y>=220&&y<285&&x<90) a->model->page=PAGE_SETTINGS;
        else if(a->model->page==PAGE_DESKTOP&&y>h-71&&y<h-24&&x>(w-206)/2+104&&x<(w-206)/2+150)a->model->page=PAGE_SETTINGS;
        else if(a->model->page==PAGE_DESKTOP&&x>w-120&&y<28) a->model->page=PAGE_CONTROL;
        else if(a->model->page==PAGE_SWITCHER) a->model->page=PAGE_SETTINGS;
        else if(y>h-15) a->model->page=PAGE_DESKTOP;
    }
    a->dirty=true;
}
static void pointer_enter(void *d,struct wl_pointer *p,uint32_t serial,struct wl_surface *s,wl_fixed_t x,wl_fixed_t y) {
    (void)p;(void)serial;(void)s;struct app *a=d;a->x=wl_fixed_to_double(x);a->y=wl_fixed_to_double(y);
}
static void pointer_leave(void *d,struct wl_pointer *p,uint32_t serial,struct wl_surface *s) {(void)d;(void)p;(void)serial;(void)s;}
static void pointer_motion(void *d,struct wl_pointer *p,uint32_t time,wl_fixed_t x,wl_fixed_t y) {
    (void)p;(void)time;struct app *a=d;a->x=wl_fixed_to_double(x);a->y=wl_fixed_to_double(y);
    if(a->down)mfw_gestures_motion(&a->gestures,-1,a->x/a->model->scale,a->y/a->model->scale);
}
static void pointer_button(void *d,struct wl_pointer *p,uint32_t serial,uint32_t ms,uint32_t button,uint32_t state) {
    (void)p;(void)serial;struct app *a=d;if(button!=BTN_LEFT)return;
    if(state==WL_POINTER_BUTTON_STATE_PRESSED){a->down=true;mfw_gestures_down(&a->gestures,-1,ms,a->x/a->model->scale,a->y/a->model->scale,a->width/a->model->scale,a->height/a->model->scale);}
    else if(a->down){a->down=false;gesture(a,mfw_gestures_up(&a->gestures,-1,ms));}
}
static void pointer_axis(void*d,struct wl_pointer*p,uint32_t t,uint32_t axis,wl_fixed_t v){(void)d;(void)p;(void)t;(void)axis;(void)v;}
static void pointer_frame(void*d,struct wl_pointer*p){(void)d;(void)p;}
static void pointer_source(void*d,struct wl_pointer*p,uint32_t s){(void)d;(void)p;(void)s;}
static void pointer_stop(void*d,struct wl_pointer*p,uint32_t t,uint32_t axis){(void)d;(void)p;(void)t;(void)axis;}
static void pointer_discrete(void*d,struct wl_pointer*p,uint32_t axis,int32_t v){(void)d;(void)p;(void)axis;(void)v;}
static const struct wl_pointer_listener pointer_listener={.enter=pointer_enter,.leave=pointer_leave,.motion=pointer_motion,
    .button=pointer_button,.axis=pointer_axis,.frame=pointer_frame,.axis_source=pointer_source,.axis_stop=pointer_stop,.axis_discrete=pointer_discrete};
static void touch_down(void*d,struct wl_touch*t,uint32_t serial,uint32_t ms,struct wl_surface*s,int32_t id,wl_fixed_t x,wl_fixed_t y){
    (void)t;(void)serial;(void)s;struct app*a=d;
    mfw_gestures_down(&a->gestures,id,ms,wl_fixed_to_double(x)/a->model->scale,wl_fixed_to_double(y)/a->model->scale,a->width/a->model->scale,a->height/a->model->scale);
}
static void touch_up(void*d,struct wl_touch*t,uint32_t serial,uint32_t ms,int32_t id){
    (void)t;(void)serial;struct app*a=d;gesture(a,mfw_gestures_up(&a->gestures,id,ms));
}
static void touch_motion(void*d,struct wl_touch*t,uint32_t ms,int32_t id,wl_fixed_t x,wl_fixed_t y){
    (void)t;(void)ms;struct app*a=d;mfw_gestures_motion(&a->gestures,id,wl_fixed_to_double(x)/a->model->scale,wl_fixed_to_double(y)/a->model->scale);
}
static void touch_frame(void*d,struct wl_touch*t){(void)d;(void)t;}
static void touch_cancel(void*d,struct wl_touch*t){(void)t;struct app*a=d;mfw_gestures_cancel(&a->gestures);a->down=false;}
static const struct wl_touch_listener touch_listener={.down=touch_down,.up=touch_up,.motion=touch_motion,.frame=touch_frame,.cancel=touch_cancel};
static void caps(void *d,struct wl_seat *seat,uint32_t caps) {
    struct app *a=d;
    if((caps&WL_SEAT_CAPABILITY_POINTER)&&!a->pointer){a->pointer=wl_seat_get_pointer(seat);wl_pointer_add_listener(a->pointer,&pointer_listener,a);}
    if(!(caps&WL_SEAT_CAPABILITY_POINTER)&&a->pointer){wl_pointer_release(a->pointer);a->pointer=NULL;}
    if((caps&WL_SEAT_CAPABILITY_TOUCH)&&!a->touch){a->touch=wl_seat_get_touch(seat);wl_touch_add_listener(a->touch,&touch_listener,a);}
    if(!(caps&WL_SEAT_CAPABILITY_TOUCH)&&a->touch){wl_touch_release(a->touch);a->touch=NULL;}
}
static void seat_name(void*d,struct wl_seat*s,const char*n){(void)d;(void)s;(void)n;}
static const struct wl_seat_listener seat_listener={.capabilities=caps,.name=seat_name};
static void global(void *data,struct wl_registry *r,uint32_t name,const char *interface,uint32_t version) {
    struct app *a=data;
    if(!strcmp(interface,"wl_compositor"))a->compositor=wl_registry_bind(r,name,&wl_compositor_interface,version<4?version:4);
    else if(!strcmp(interface,"wl_shm"))a->shm=wl_registry_bind(r,name,&wl_shm_interface,1);
    else if(!strcmp(interface,"xdg_wm_base")){a->wm=wl_registry_bind(r,name,&xdg_wm_base_interface,1);xdg_wm_base_add_listener(a->wm,&wm_listener,a);}
    else if(!strcmp(interface,"zxdg_decoration_manager_v1"))a->decoration_manager=wl_registry_bind(r,name,&zxdg_decoration_manager_v1_interface,1);
    else if(!strcmp(interface,"wl_seat")&&!a->seat&&version>=5){a->seat=wl_registry_bind(r,name,&wl_seat_interface,5);wl_seat_add_listener(a->seat,&seat_listener,a);}
}
static void global_remove(void*d,struct wl_registry*r,uint32_t name){(void)d;(void)r;(void)name;}
static const struct wl_registry_listener registry_listener={.global=global,.global_remove=global_remove};
int mfw_wayland(struct mfw_model *model,bool windowed,int limit_frames) {
    struct app a={.model=model,.width=720,.height=1440,.dirty=true,.running=true,.windowed=windowed,.limit=limit_frames};
    a.display=wl_display_connect(NULL);if(!a.display){fprintf(stderr,"No Wayland compositor available\n");return 1;}
    struct wl_registry *registry=wl_display_get_registry(a.display);wl_registry_add_listener(registry,&registry_listener,&a);
    if(wl_display_roundtrip(a.display)<0||!a.compositor||!a.shm||!a.wm){fprintf(stderr,"Missing Wayland globals\n");wl_display_disconnect(a.display);return 1;}
    a.surface=wl_compositor_create_surface(a.compositor);a.xdg=xdg_wm_base_get_xdg_surface(a.wm,a.surface);
    xdg_surface_add_listener(a.xdg,&surface_listener,&a);a.top=xdg_surface_get_toplevel(a.xdg);
    xdg_toplevel_add_listener(a.top,&top_listener,&a);xdg_toplevel_set_title(a.top,model->page==PAGE_SETTINGS?"Settings":"MICROFW Preview");
    xdg_toplevel_set_app_id(a.top,model->page==PAGE_SETTINGS?"org.microfw.Settings":"org.microfw.Preview");
    xdg_toplevel_set_min_size(a.top,(int)(320*model->scale),(int)(400*model->scale));
    if(a.decoration_manager){a.decoration=zxdg_decoration_manager_v1_get_toplevel_decoration(a.decoration_manager,a.top);
        zxdg_toplevel_decoration_v1_add_listener(a.decoration,&decoration_listener,&a);
        zxdg_toplevel_decoration_v1_set_mode(a.decoration,ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);}
    if(!windowed)xdg_toplevel_set_fullscreen(a.top,NULL);
    wl_surface_commit(a.surface);signal(SIGINT,stop);signal(SIGTERM,stop);
    time_t last=0;
    while(a.running&&!stopped) {
        if(wl_display_dispatch_pending(a.display)<0)break;
        time_t now=time(NULL);if(now!=last){last=now;mfw_sampler_read(model);a.dirty=true;}
        draw(&a);if(a.limit&&a.frames>=a.limit&&!a.frame)break;
        if(wl_display_flush(a.display)<0&&errno!=EAGAIN)break;
        struct pollfd fd={.fd=wl_display_get_fd(a.display),.events=POLLIN};
        int ret=poll(&fd,1,100);if(ret<0&&errno!=EINTR)break;
        if(ret>0&&(fd.revents&(POLLERR|POLLHUP)))break;
        if(ret>0&&(fd.revents&POLLIN)&&wl_display_dispatch(a.display)<0)break;
    }
    mfw_motion_finish(&a.motion);
    if(a.frame)wl_callback_destroy(a.frame);
    if(a.touch)wl_touch_release(a.touch);
    if(a.pointer)wl_pointer_release(a.pointer);
    if(a.seat)wl_seat_release(a.seat);
    if(a.decoration)zxdg_toplevel_decoration_v1_destroy(a.decoration);
    if(a.decoration_manager)zxdg_decoration_manager_v1_destroy(a.decoration_manager);
    xdg_toplevel_destroy(a.top);xdg_surface_destroy(a.xdg);wl_surface_destroy(a.surface);
    for(int i=0;i<3;i++)buffer_free(&a.buffers[i]);
    xdg_wm_base_destroy(a.wm);wl_shm_destroy(a.shm);wl_compositor_destroy(a.compositor);wl_registry_destroy(registry);
    int result=wl_display_get_error(a.display)?1:0;wl_display_disconnect(a.display);return result;
}
