#define _GNU_SOURCE
#include "ui.h"
#include "motion.h"
#include "sampler.h"
#include "gestures.h"
#include "wlr-layer-shell-client-protocol.h"
#include "wlr-foreign-toplevel-management-client-protocol.h"
#include <wayland-client.h>
#include <sys/mman.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <spawn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <linux/input-event-codes.h>
extern char **environ;

struct desktop;
struct dbuf{struct wl_buffer*wl;void*data;size_t size;int w,h;bool busy;};
struct view{struct desktop*d;unsigned kind;struct wl_surface*surface;struct zwlr_layer_surface_v1*layer;
    struct wl_callback*frame;int w,h;bool configured,dirty;unsigned commits,presents;struct dbuf buffers[2];};
struct foreign{struct desktop*d;struct zwlr_foreign_toplevel_handle_v1*handle;struct mfw_window_info info;bool policy_sent;};
struct touch_origin{bool used;int32_t id;double y;};
struct desktop{struct mfw_motion motion;struct wl_display*display;struct wl_compositor*compositor;struct wl_shm*shm;struct zwlr_layer_shell_v1*layers;
    struct zwlr_foreign_toplevel_manager_v1*manager;struct wl_seat*seat;struct wl_pointer*pointer;struct wl_touch*touch;
    struct view views[3];struct foreign windows[16];struct mfw_model*model;struct mfw_gestures gestures;
    struct touch_origin origins[10];struct view*pointer_view;double px,py,pointer_origin;
    int w,h,frames,limit,smoke_stage,smoke_frame_baseline;bool run,dirty,pointer_down,smoke;struct foreign*smoke_target;char executable[4096];};
static volatile sig_atomic_t interrupted;
static void interrupt(int sig){(void)sig;interrupted=1;}
static void sync_windows(struct desktop*d){
    d->model->n_windows=0;
    for(unsigned i=0;i<16;i++)if(d->windows[i].handle)d->model->windows[d->model->n_windows++]=d->windows[i].info;
    d->dirty=true;
}
static struct foreign*active(struct desktop*d){
    struct foreign*first=NULL;
    for(unsigned i=0;i<16;i++)if(d->windows[i].handle){if(!first)first=&d->windows[i];if(d->windows[i].info.active)return &d->windows[i];}
    return first;
}
static void title(void*p,struct zwlr_foreign_toplevel_handle_v1*h,const char*s){(void)h;struct foreign*f=p;snprintf(f->info.title,sizeof f->info.title,"%s",s);}
static void app_id(void*p,struct zwlr_foreign_toplevel_handle_v1*h,const char*s){(void)h;struct foreign*f=p;snprintf(f->info.app_id,sizeof f->info.app_id,"%s",s);}
static void output_enter(void*p,struct zwlr_foreign_toplevel_handle_v1*h,struct wl_output*o){(void)p;(void)h;(void)o;}
static void output_leave(void*p,struct zwlr_foreign_toplevel_handle_v1*h,struct wl_output*o){(void)p;(void)h;(void)o;}
static void state(void*p,struct zwlr_foreign_toplevel_handle_v1*h,struct wl_array*states){
    (void)h;struct foreign*f=p;f->info.active=f->info.minimized=f->info.maximized=f->info.fullscreen=false;
    uint32_t*v;wl_array_for_each(v,states){if(*v==ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_ACTIVATED)f->info.active=true;
        if(*v==ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_MINIMIZED)f->info.minimized=true;
        if(*v==ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_MAXIMIZED)f->info.maximized=true;
        if(*v==ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_FULLSCREEN)f->info.fullscreen=true;}
}
static void done(void*p,struct zwlr_foreign_toplevel_handle_v1*h){
    struct foreign*f=p;
    if(!f->policy_sent){f->policy_sent=true;zwlr_foreign_toplevel_handle_v1_set_fullscreen(h,NULL);}
    fprintf(stderr,"TOPLEVEL id=%u title=%s active=%u minimized=%u fullscreen=%u\n",f->info.id,f->info.title,f->info.active,f->info.minimized,f->info.fullscreen);
    sync_windows(f->d);
}
static void closed(void*p,struct zwlr_foreign_toplevel_handle_v1*h){struct foreign*f=p;fprintf(stderr,"TOPLEVEL_CLOSED id=%u\n",f->info.id);zwlr_foreign_toplevel_handle_v1_destroy(h);f->handle=NULL;sync_windows(f->d);}
static void parent(void*p,struct zwlr_foreign_toplevel_handle_v1*h,struct zwlr_foreign_toplevel_handle_v1*q){(void)p;(void)h;(void)q;}
static const struct zwlr_foreign_toplevel_handle_v1_listener foreign_listener={.title=title,.app_id=app_id,.output_enter=output_enter,.output_leave=output_leave,.state=state,.done=done,.closed=closed,.parent=parent};
static void toplevel(void*p,struct zwlr_foreign_toplevel_manager_v1*m,struct zwlr_foreign_toplevel_handle_v1*h){
    (void)m;struct desktop*d=p;
    for(unsigned i=0;i<16;i++)if(!d->windows[i].handle){struct foreign*f=&d->windows[i];memset(f,0,sizeof *f);f->d=d;f->handle=h;f->info.id=i+1;zwlr_foreign_toplevel_handle_v1_add_listener(h,&foreign_listener,f);return;}
    zwlr_foreign_toplevel_handle_v1_destroy(h);
}
static void finished(void*p,struct zwlr_foreign_toplevel_manager_v1*m){(void)m;((struct desktop*)p)->run=false;}
static const struct zwlr_foreign_toplevel_manager_v1_listener manager_listener={.toplevel=toplevel,.finished=finished};
static void free_buffer(struct dbuf*b){if(b->wl)wl_buffer_destroy(b->wl);if(b->data)munmap(b->data,b->size);memset(b,0,sizeof *b);}
static void buffer_release(void*p,struct wl_buffer*b){(void)b;((struct dbuf*)p)->busy=false;}
static const struct wl_buffer_listener buffer_listener={.release=buffer_release};
static bool alloc_buffer(struct view*v,struct dbuf*b){
    free_buffer(b);b->size=(size_t)v->w*v->h*4;int fd=memfd_create("microfw-desktop",MFD_CLOEXEC);if(fd<0)return false;
    if(ftruncate(fd,(off_t)b->size)<0){close(fd);return false;}
    b->data=mmap(NULL,b->size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);if(b->data==MAP_FAILED){b->data=NULL;close(fd);return false;}
    struct wl_shm_pool*pool=wl_shm_create_pool(v->d->shm,fd,(int)b->size);
    b->wl=wl_shm_pool_create_buffer(pool,0,v->w,v->h,v->w*4,WL_SHM_FORMAT_XRGB8888);wl_shm_pool_destroy(pool);close(fd);
    b->w=v->w;b->h=v->h;wl_buffer_add_listener(b->wl,&buffer_listener,b);return true;
}
static void frame_done(void*p,struct wl_callback*c,uint32_t time){(void)time;struct view*v=p;wl_callback_destroy(c);v->frame=NULL;
    if(++v->presents==1)fprintf(stderr,"LAYER_FRAME_DONE kind=%u\n",v->kind);}
static const struct wl_callback_listener frame_listener={.done=frame_done};
static void draw_view(struct view*v){
    if(!v->configured||v->frame||!v->dirty)return;
    struct dbuf*b=NULL;for(unsigned i=0;i<2;i++)if(!v->buffers[i].busy){b=&v->buffers[i];break;}if(!b)return;
    if((!b->wl||b->w!=v->w||b->h!=v->h)&&!alloc_buffer(v,b)){v->d->run=false;return;}
    cairo_surface_t*s=cairo_image_surface_create_for_data(b->data,CAIRO_FORMAT_RGB24,b->w,b->h,b->w*4);cairo_t*c=cairo_create(s);
    if(v->kind==2)cairo_translate(c,0,-(v->d->h-v->h));
    bool animating=false;
    if(v->kind==0)animating=mfw_motion_render(&v->d->motion,c,v->d->w,v->d->h,v->d->model,mfw_motion_now());else mfw_render(c,v->d->w,v->d->h,v->d->model);cairo_destroy(c);cairo_surface_flush(s);cairo_surface_destroy(s);
    v->frame=wl_surface_frame(v->surface);wl_callback_add_listener(v->frame,&frame_listener,v);b->busy=true;
    wl_surface_attach(v->surface,b->wl,0,0);wl_surface_damage(v->surface,0,0,v->w,v->h);wl_surface_commit(v->surface);
    if(++v->commits==1)fprintf(stderr,"LAYER_BUFFER_COMMITTED kind=%u size=%dx%d\n",v->kind,v->w,v->h);
    v->dirty=animating;
    if(!v->kind)v->d->frames++;
}
static void configure(void*p,struct zwlr_layer_surface_v1*l,uint32_t serial,uint32_t w,uint32_t h){
    struct view*v=p;zwlr_layer_surface_v1_ack_configure(l,serial);
    if(!w||!h||w>4096||h>4096){v->d->run=false;return;}
    v->w=(int)w;v->h=(int)h;v->configured=true;v->d->dirty=true;
    if(!v->kind){v->d->w=v->w;v->d->h=v->h;fprintf(stderr,"DESKTOP_CONFIGURED %dx%d\n",v->w,v->h);}
}
static void layer_closed(void*p,struct zwlr_layer_surface_v1*l){(void)l;((struct view*)p)->d->run=false;}
static const struct zwlr_layer_surface_v1_listener layer_listener={.configure=configure,.closed=layer_closed};
static double origin(struct desktop*d,struct view*v){return v&&v->kind==2?d->h-v->h:0;}
static struct view*find_view(struct desktop*d,struct wl_surface*s){for(unsigned i=0;i<3;i++)if(d->views[i].surface==s)return &d->views[i];return NULL;}
static void page(struct desktop*d,enum mfw_page p){
    d->model->page=p;
    zwlr_layer_surface_v1_set_layer(d->views[0].layer,p==PAGE_DESKTOP?ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND:ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY);
    /* An occluded background may have a pending frame callback indefinitely.
     * Commit the role change now; visibility can then release that callback. */
    wl_surface_commit(d->views[0].surface);d->dirty=true;
}
static void launch(struct desktop*d,bool settings){
    pid_t pid;int result;
    if(settings){char*argv[]={d->executable,"--page","settings",d->model->demo?"--demo":NULL,NULL};result=posix_spawn(&pid,d->executable,NULL,NULL,argv,environ);}
    else{
        char*argv[]={"foot",NULL};result=posix_spawnp(&pid,argv[0],NULL,NULL,argv,environ);
        if(result==ENOENT){argv[0]="weston-terminal";result=posix_spawnp(&pid,argv[0],NULL,NULL,argv,environ);}
    }
    fprintf(stderr,"LAUNCH %s result=%d\n",settings?"Settings":"Terminal",result);
}
static void act(struct desktop*d,struct mfw_gesture_result r){
    double scale=d->model->scale,w=d->w/scale,h=d->h/scale,x=r.x,y=r.y;
    if(r.action==ACTION_DESKTOP){for(unsigned i=0;i<16;i++)if(d->windows[i].handle)zwlr_foreign_toplevel_handle_v1_set_minimized(d->windows[i].handle);page(d,PAGE_DESKTOP);}
    else if(r.action==ACTION_OVERVIEW)page(d,PAGE_SWITCHER);
    else if(r.action==ACTION_CONTROL)page(d,PAGE_CONTROL);
    else if(r.action==ACTION_LEAVE_FULLSCREEN){struct foreign*f=active(d);if(f)zwlr_foreign_toplevel_handle_v1_unset_fullscreen(f->handle);}
    else if(r.action==ACTION_TAP){
        if(d->model->page==PAGE_SWITCHER&&y>=96){unsigned n=(unsigned)((y-96)/58);if(n<d->model->n_windows){unsigned id=d->model->windows[n].id;struct foreign*f=&d->windows[id-1];
            if(x>(w+fmin(480,w-40))/2-50)zwlr_foreign_toplevel_handle_v1_close(f->handle);
            else{zwlr_foreign_toplevel_handle_v1_unset_minimized(f->handle);if(d->seat)zwlr_foreign_toplevel_handle_v1_activate(f->handle,d->seat);page(d,PAGE_DESKTOP);}}}
        else if(y<28&&x>w-120)page(d,PAGE_CONTROL);
        else if(y>h-16)page(d,PAGE_DESKTOP);
        else if(d->model->page==PAGE_DESKTOP){double dx=(w-206)/2;
            if((x<90&&y>=220&&y<285)||(y>h-71&&y<h-24&&x>dx+104&&x<dx+151))launch(d,true);
            else if((x<90&&y>=135&&y<205)||(y>h-71&&y<h-24&&x>dx+55&&x<dx+103))launch(d,false);
            else if(y>h-71&&y<h-24&&x>dx+152&&x<dx+201)page(d,PAGE_SWITCHER);
        }
    }
    d->dirty=true;
}
static void pointer_enter(void*p,struct wl_pointer*q,uint32_t serial,struct wl_surface*s,wl_fixed_t x,wl_fixed_t y){(void)q;(void)serial;struct desktop*d=p;d->pointer_view=find_view(d,s);d->px=wl_fixed_to_double(x);d->py=wl_fixed_to_double(y);}
static void pointer_leave(void*p,struct wl_pointer*q,uint32_t serial,struct wl_surface*s){(void)p;(void)q;(void)serial;(void)s;}
static void pointer_motion(void*p,struct wl_pointer*q,uint32_t ms,wl_fixed_t x,wl_fixed_t y){(void)q;(void)ms;struct desktop*d=p;d->px=wl_fixed_to_double(x);d->py=wl_fixed_to_double(y);if(d->pointer_down)mfw_gestures_motion(&d->gestures,-1,d->px/d->model->scale,(d->py+d->pointer_origin)/d->model->scale);}
static void pointer_button(void*p,struct wl_pointer*q,uint32_t serial,uint32_t ms,uint32_t button,uint32_t state){
    (void)q;(void)serial;struct desktop*d=p;if(button!=BTN_LEFT)return;
    if(state){d->pointer_down=true;d->pointer_origin=origin(d,d->pointer_view);mfw_gestures_down(&d->gestures,-1,ms,d->px/d->model->scale,(d->py+d->pointer_origin)/d->model->scale,d->w/d->model->scale,d->h/d->model->scale);}
    else if(d->pointer_down){d->pointer_down=false;act(d,mfw_gestures_up(&d->gestures,-1,ms));}
}
static void pointer_axis(void*p,struct wl_pointer*q,uint32_t ms,uint32_t axis,wl_fixed_t value){(void)p;(void)q;(void)ms;(void)axis;(void)value;}
static const struct wl_pointer_listener pointer_listener={.enter=pointer_enter,.leave=pointer_leave,.motion=pointer_motion,.button=pointer_button,.axis=pointer_axis};
static struct touch_origin*touch_origin(struct desktop*d,int32_t id){for(unsigned i=0;i<10;i++)if(d->origins[i].used&&d->origins[i].id==id)return &d->origins[i];return NULL;}
static void touch_down(void*p,struct wl_touch*t,uint32_t serial,uint32_t ms,struct wl_surface*s,int32_t id,wl_fixed_t x,wl_fixed_t y){
    (void)t;(void)serial;struct desktop*d=p;struct touch_origin*o=NULL;
    for(unsigned i=0;i<10;i++)if(!d->origins[i].used){o=&d->origins[i];break;}
    if(!o)return;
    *o=(struct touch_origin){.used=true,.id=id,.y=origin(d,find_view(d,s))};
    mfw_gestures_down(&d->gestures,id,ms,wl_fixed_to_double(x)/d->model->scale,(wl_fixed_to_double(y)+o->y)/d->model->scale,d->w/d->model->scale,d->h/d->model->scale);
}
static void touch_up(void*p,struct wl_touch*t,uint32_t serial,uint32_t ms,int32_t id){(void)t;(void)serial;struct desktop*d=p;struct touch_origin*o=touch_origin(d,id);if(o)o->used=false;act(d,mfw_gestures_up(&d->gestures,id,ms));}
static void touch_motion(void*p,struct wl_touch*t,uint32_t ms,int32_t id,wl_fixed_t x,wl_fixed_t y){(void)t;(void)ms;struct desktop*d=p;struct touch_origin*o=touch_origin(d,id);if(o)mfw_gestures_motion(&d->gestures,id,wl_fixed_to_double(x)/d->model->scale,(wl_fixed_to_double(y)+o->y)/d->model->scale);}
static void touch_frame(void*p,struct wl_touch*t){(void)p;(void)t;}
static void touch_cancel(void*p,struct wl_touch*t){(void)t;struct desktop*d=p;mfw_gestures_cancel(&d->gestures);memset(d->origins,0,sizeof d->origins);}
static const struct wl_touch_listener touch_listener={.down=touch_down,.up=touch_up,.motion=touch_motion,.frame=touch_frame,.cancel=touch_cancel};
static void capabilities(void*p,struct wl_seat*s,uint32_t caps){struct desktop*d=p;
    if((caps&WL_SEAT_CAPABILITY_POINTER)&&!d->pointer){d->pointer=wl_seat_get_pointer(s);wl_pointer_add_listener(d->pointer,&pointer_listener,d);}
    if(!(caps&WL_SEAT_CAPABILITY_POINTER)&&d->pointer){wl_pointer_destroy(d->pointer);d->pointer=NULL;d->pointer_down=false;mfw_gestures_cancel(&d->gestures);}
    if((caps&WL_SEAT_CAPABILITY_TOUCH)&&!d->touch){d->touch=wl_seat_get_touch(s);wl_touch_add_listener(d->touch,&touch_listener,d);}
    if(!(caps&WL_SEAT_CAPABILITY_TOUCH)&&d->touch){wl_touch_destroy(d->touch);d->touch=NULL;mfw_gestures_cancel(&d->gestures);memset(d->origins,0,sizeof d->origins);}}
static const struct wl_seat_listener seat_listener={.capabilities=capabilities};
static void global(void*p,struct wl_registry*r,uint32_t id,const char*name,uint32_t version){struct desktop*d=p;
    if(!strcmp(name,"wl_compositor")&&version>=4)d->compositor=wl_registry_bind(r,id,&wl_compositor_interface,4);
    else if(!strcmp(name,"wl_shm"))d->shm=wl_registry_bind(r,id,&wl_shm_interface,1);
    else if(!strcmp(name,"zwlr_layer_shell_v1")&&version>=2)d->layers=wl_registry_bind(r,id,&zwlr_layer_shell_v1_interface,2);
    else if(!strcmp(name,"zwlr_foreign_toplevel_manager_v1")&&version>=3){d->manager=wl_registry_bind(r,id,&zwlr_foreign_toplevel_manager_v1_interface,3);zwlr_foreign_toplevel_manager_v1_add_listener(d->manager,&manager_listener,d);}
    else if(!strcmp(name,"wl_seat")&&!d->seat){d->seat=wl_registry_bind(r,id,&wl_seat_interface,1);wl_seat_add_listener(d->seat,&seat_listener,d);}}
static void global_remove(void*p,struct wl_registry*r,uint32_t id){(void)p;(void)r;(void)id;}
static const struct wl_registry_listener registry_listener={.global=global,.global_remove=global_remove};
static void smoke(struct desktop*d){
    if(!d->smoke)return;
    if(!d->smoke_stage&&d->model->n_windows>=2){d->smoke_target=NULL;
        for(unsigned i=0;i<16;i++)if(d->windows[i].handle&&!strcmp(d->windows[i].info.app_id,"org.microfw.Settings"))d->smoke_target=&d->windows[i];
        if(d->smoke_target&&d->smoke_target->info.fullscreen){d->smoke_frame_baseline=d->frames;page(d,PAGE_SWITCHER);d->smoke_stage=-1;fprintf(stderr,"SMOKE_OVERVIEW_REQUEST\n");}}
    else if(d->smoke_stage==-1&&d->frames>d->smoke_frame_baseline&&!d->views[0].frame){page(d,PAGE_DESKTOP);zwlr_foreign_toplevel_handle_v1_unset_fullscreen(d->smoke_target->handle);d->smoke_stage=1;fprintf(stderr,"SMOKE_OVERVIEW_PRESENTED_RESTORE_REQUEST\n");}
    else if(d->smoke_stage==1&&d->smoke_target->handle&&!d->smoke_target->info.fullscreen){zwlr_foreign_toplevel_handle_v1_set_minimized(d->smoke_target->handle);d->smoke_stage=2;fprintf(stderr,"SMOKE_RESTORE_ACK_MINIMIZE_REQUEST\n");}
    else if(d->smoke_stage==2&&d->smoke_target->handle&&d->smoke_target->info.minimized){zwlr_foreign_toplevel_handle_v1_unset_minimized(d->smoke_target->handle);if(d->seat)zwlr_foreign_toplevel_handle_v1_activate(d->smoke_target->handle,d->seat);d->smoke_stage=3;fprintf(stderr,"SMOKE_MINIMIZE_ACK_ACTIVATE_REQUEST\n");}
    else if(d->smoke_stage==3&&d->smoke_target->handle&&!d->smoke_target->info.minimized&&d->smoke_target->info.active){zwlr_foreign_toplevel_handle_v1_set_maximized(d->smoke_target->handle);d->smoke_stage=4;fprintf(stderr,"SMOKE_ACTIVATE_ACK_MAXIMIZE_REQUEST\n");}
    else if(d->smoke_stage==4&&d->smoke_target->handle&&d->smoke_target->info.maximized){zwlr_foreign_toplevel_handle_v1_unset_maximized(d->smoke_target->handle);d->smoke_stage=5;fprintf(stderr,"SMOKE_MAXIMIZE_ACK_UNMAXIMIZE_REQUEST\n");}
    else if(d->smoke_stage==5&&d->smoke_target->handle&&!d->smoke_target->info.maximized){zwlr_foreign_toplevel_handle_v1_close(d->smoke_target->handle);d->smoke_stage=6;fprintf(stderr,"SMOKE_UNMAXIMIZE_ACK_CLOSE_REQUEST\n");}
    else if(d->smoke_stage==6&&!d->smoke_target->handle){d->smoke_stage=7;fprintf(stderr,"SMOKE_CLOSE_ACK_PASS\n");}
}
int mfw_desktop(struct mfw_model*model,int limit,bool smoke_actions){
    struct desktop d={.model=model,.w=720,.h=1440,.limit=limit,.run=true,.dirty=true,.smoke=smoke_actions};
    ssize_t n=readlink("/proc/self/exe",d.executable,sizeof d.executable-1);if(n<0)return 1;d.executable[n]=0;
    d.display=wl_display_connect(NULL);if(!d.display)return 1;
    struct wl_registry*r=wl_display_get_registry(d.display);wl_registry_add_listener(r,&registry_listener,&d);
    if(wl_display_roundtrip(d.display)<0||!d.compositor||!d.shm||!d.layers||!d.manager){fprintf(stderr,"Desktop requires layer-shell v2 and foreign-toplevel v3\n");wl_display_disconnect(d.display);return 1;}
    for(unsigned i=0;i<3;i++){struct view*v=&d.views[i];v->d=&d;v->kind=i;v->surface=wl_compositor_create_surface(d.compositor);
        v->layer=zwlr_layer_shell_v1_get_layer_surface(d.layers,v->surface,NULL,i?ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY:ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND,i==0?"microfw-desktop":i==1?"microfw-status":"microfw-home");
        zwlr_layer_surface_v1_add_listener(v->layer,&layer_listener,v);
        uint32_t anchor=ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT|ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT;
        if(i!=2)anchor|=ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP;
        if(i!=1)anchor|=ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM;
        zwlr_layer_surface_v1_set_anchor(v->layer,anchor);zwlr_layer_surface_v1_set_size(v->layer,0,i?(uint32_t)((i==1?28:24)*model->scale):0);
        zwlr_layer_surface_v1_set_exclusive_zone(v->layer,i?(int)((i==1?28:24)*model->scale):-1);wl_surface_commit(v->surface);}
    signal(SIGINT,interrupt);signal(SIGTERM,interrupt);signal(SIGCHLD,SIG_IGN);time_t last=0;
    while(d.run&&!interrupted){if(wl_display_dispatch_pending(d.display)<0)break;time_t now=time(NULL);if(now!=last){
        last=now;char clock[sizeof model->clock];memcpy(clock,model->clock,sizeof clock);
        int capacity=model->capacity;bool charging=model->charging,wifi=model->wifi;
        mfw_sampler_read(model);
        /* A sensor sample is not a reason to repaint the entire desktop.
         * Only status-visible changes damage that strip; page/window events
         * independently set d.dirty for actual desktop content changes. */
        if(memcmp(clock,model->clock,sizeof clock)||capacity!=model->capacity||charging!=model->charging||wifi!=model->wifi)d.views[1].dirty=true;
    }smoke(&d);
        if(d.dirty){for(unsigned i=0;i<3;i++)d.views[i].dirty=true;d.dirty=false;}
        for(unsigned i=0;i<3;i++)draw_view(&d.views[i]);
        if((d.smoke&&d.smoke_stage==7)||(!d.smoke&&d.limit&&d.frames>=d.limit))break;
        if(wl_display_flush(d.display)<0&&errno!=EAGAIN)break;
        struct pollfd fd={.fd=wl_display_get_fd(d.display),.events=POLLIN};int ret=poll(&fd,1,50);
        if(ret<0&&errno!=EINTR)break;
        if(ret>0&&(fd.revents&(POLLERR|POLLHUP)))break;
        if(ret>0&&(fd.revents&POLLIN)&&wl_display_dispatch(d.display)<0)break;}
    mfw_motion_finish(&d.motion);
    for(unsigned i=0;i<16;i++)if(d.windows[i].handle)zwlr_foreign_toplevel_handle_v1_destroy(d.windows[i].handle);
    for(unsigned i=0;i<3;i++){struct view*v=&d.views[i];if(v->frame)wl_callback_destroy(v->frame);for(unsigned j=0;j<2;j++)free_buffer(&v->buffers[j]);zwlr_layer_surface_v1_destroy(v->layer);wl_surface_destroy(v->surface);}
    if(d.touch)wl_touch_destroy(d.touch);
    if(d.pointer)wl_pointer_destroy(d.pointer);
    if(d.seat)wl_seat_destroy(d.seat);
    zwlr_foreign_toplevel_manager_v1_destroy(d.manager);zwlr_layer_shell_v1_destroy(d.layers);wl_shm_destroy(d.shm);wl_compositor_destroy(d.compositor);wl_registry_destroy(r);
    int result=wl_display_get_error(d.display)|| (d.smoke&&d.smoke_stage!=7);wl_display_disconnect(d.display);return result?1:0;
}
