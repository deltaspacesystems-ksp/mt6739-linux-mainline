#define _POSIX_C_SOURCE 200809L
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
 double radius=28*sin(3.141592653589793*p);
 cairo_save(c);cairo_new_sub_path(c);
 cairo_arc(c,shift+a->w-radius,radius,radius,-1.570796326794897,0);
 cairo_arc(c,shift+a->w-radius,a->h-radius,radius,0,1.570796326794897);
 cairo_arc(c,shift+radius,a->h-radius,radius,1.570796326794897,3.141592653589793);
 cairo_arc(c,shift+radius,radius,radius,3.141592653589793,4.71238898038469);
 cairo_close_path(c);cairo_clip(c);
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
