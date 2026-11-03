from pathlib import Path
r=Path('X:/y5linux/phone/ui')
(r/'src/motion.h').write_text('''#ifndef MFW_MOTION_H
#define MFW_MOTION_H
#include "ui.h"
struct mfw_motion {cairo_surface_t *from,*to;struct mfw_model last;int w,h;double start;bool initialized,active;};
double mfw_motion_now(void);
void mfw_motion_finish(struct mfw_motion *);
bool mfw_motion_render(struct mfw_motion *,cairo_t *,int,int,const struct mfw_model *,double);
#endif
''',encoding='utf-8')
(r/'src/motion.c').write_text('''#define _POSIX_C_SOURCE 200809L
#include "motion.h"
#include <math.h>
#include <time.h>
/* Cached endpoint textures: no Pango/layout per animation frame, no idle loop.
 * Critically damped response, normalized to exactly finish after 420ms. */
double mfw_motion_now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec/1e9;}
void mfw_motion_finish(struct mfw_motion *a){if(a->from)cairo_surface_destroy(a->from);if(a->to)cairo_surface_destroy(a->to);a->from=a->to=NULL;a->active=false;}
static double progress(double t){if(t<=0)return 0;if(t>=.42)return 1;double k=18,end=1-(1+k*.42)*exp(-k*.42);return (1-(1+k*t)*exp(-k*t))/end;}
static cairo_surface_t *image(int w,int h){cairo_surface_t *s=cairo_image_surface_create(CAIRO_FORMAT_RGB24,w,h);if(cairo_surface_status(s)){cairo_surface_destroy(s);return NULL;}return s;}
static void paint(struct mfw_motion *a,cairo_t *c,double now){
 double p=progress(now-a->start),shift=a->w*(1-p);
 cairo_save(c);cairo_set_source_rgb(c,0,0,0);cairo_paint(c);
 cairo_set_source_surface(c,a->from,-a->w*.20*p,0);cairo_paint(c);
 cairo_set_source_rgba(c,0,0,0,.24*p);cairo_paint(c);
 cairo_save(c);cairo_rectangle(c,shift,0,a->w,a->h);cairo_clip(c);
 cairo_set_source_surface(c,a->to,shift,0);cairo_paint(c);cairo_restore(c);cairo_restore(c);
}
bool mfw_motion_render(struct mfw_motion *a,cairo_t *c,int w,int h,const struct mfw_model *m,double now){
 if(!isfinite(now)||w<1||h<1||w>4096||h>4096||(double)w*h>4194304){mfw_motion_finish(a);a->initialized=false;mfw_render(c,w,h,m);return false;}
 if(a->initialized&&(w!=a->w||h!=a->h)){mfw_motion_finish(a);a->initialized=false;}
 if(a->initialized&&m->page!=a->last.page){
  cairo_surface_t *from=image(w,h),*to=image(w,h);
  if(from&&to){
   cairo_t *t=cairo_create(from);if(a->active)paint(a,t,now);else mfw_render(t,w,h,&a->last);cairo_destroy(t);
   t=cairo_create(to);mfw_render(t,w,h,m);cairo_destroy(t);
   mfw_motion_finish(a);a->from=from;a->to=to;a->start=now;a->active=true;
  }else{if(from)cairo_surface_destroy(from);if(to)cairo_surface_destroy(to);mfw_motion_finish(a);}
 }
 a->last=*m;a->w=w;a->h=h;a->initialized=true;
 if(a->active&&now-a->start>=.42)mfw_motion_finish(a);
 if(a->active)paint(a,c,now);else mfw_render(c,w,h,m);
 return a->active;
}
''',encoding='utf-8')
p=r/'Makefile';s=p.read_text(encoding='utf-8').replace('src/main.c src/model.c','src/main.c src/motion.c src/model.c');p.write_text(s,encoding='utf-8',newline='\n')
p=r/'src/wayland.c';s=p.read_text(encoding='utf-8').replace('#include "ui.h"','#include "ui.h"\n#include "motion.h"',1).replace('struct mfw_model *model; int width','struct mfw_motion motion; struct mfw_model *model; int width').replace('cairo_t *c=cairo_create(s);mfw_render(c,b->w,b->h,a->model);','cairo_t *c=cairo_create(s);bool animating=mfw_motion_render(&a->motion,c,b->w,b->h,a->model,mfw_motion_now());').replace('a->dirty=false;a->frames++;','a->dirty=animating;a->frames++;').replace('    if(a.frame)wl_callback_destroy(a.frame);','    mfw_motion_finish(&a.motion);\n    if(a.frame)wl_callback_destroy(a.frame);');p.write_text(s,encoding='utf-8',newline='\n')
p=r/'src/desktop.c';s=p.read_text(encoding='utf-8').replace('#include "ui.h"','#include "ui.h"\n#include "motion.h"',1).replace('struct desktop{struct wl_display','struct desktop{struct mfw_motion motion;struct wl_display');s=s.replace('mfw_render(c,v->d->w,v->d->h,v->d->model);','bool animating=false;\n    if(v->kind==0)animating=mfw_motion_render(&v->d->motion,c,v->d->w,v->d->h,v->d->model,mfw_motion_now());else mfw_render(c,v->d->w,v->d->h,v->d->model);').replace('v->dirty=false;','v->dirty=animating;');s=s.replace('    for(unsigned i=0;i<16;i++)if(d.windows[i].handle)zwlr_foreign_toplevel_handle_v1_destroy','    mfw_motion_finish(&d.motion);\n    for(unsigned i=0;i<16;i++)if(d.windows[i].handle)zwlr_foreign_toplevel_handle_v1_destroy');p.write_text(s,encoding='utf-8',newline='\n')
for name in ['src/motion.h','src/motion.c','src/wayland.c','src/desktop.c','Makefile']:
 Path('D:/y5_linux_work/microfw-ui',name).write_bytes((r/name).read_bytes())
