#include "motion.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
void mfw_render(cairo_t*c,int w,int h,const struct mfw_model*m){(void)w;(void)h;cairo_set_source_rgb(c,m->page==PAGE_SETTINGS?1:0,m->page==PAGE_CONTROL?1:0,m->page==PAGE_DESKTOP?1:0);cairo_paint(c);}
static uint32_t pixel(cairo_surface_t*s){cairo_surface_flush(s);return *(uint32_t*)cairo_image_surface_get_data(s);}
int main(void){
 struct mfw_motion a={0};struct mfw_model m={.page=PAGE_DESKTOP};
 cairo_surface_t*s=cairo_image_surface_create(CAIRO_FORMAT_RGB24,100,100);cairo_t*c=cairo_create(s);
 assert(!mfw_motion_render(&a,c,100,100,&m,0));uint32_t old=pixel(s);m.page=PAGE_SETTINGS;
 assert(mfw_motion_render(&a,c,100,100,&m,1)&&pixel(s)==old);
 assert(mfw_motion_render(&a,c,100,100,&m,1.15));unsigned char before[40000];cairo_surface_flush(s);memcpy(before,cairo_image_surface_get_data(s),sizeof(before));
 m.page=PAGE_CONTROL;assert(mfw_motion_render(&a,c,100,100,&m,1.15));cairo_surface_flush(s);assert(!memcmp(before,cairo_image_surface_get_data(s),sizeof(before)));
 assert(!mfw_motion_render(&a,c,100,100,&m,2));assert((pixel(s)&0xffffff)==0x0000ff00);assert(!a.from&&!a.to);
 m.page=PAGE_SETTINGS;assert(mfw_motion_render(&a,c,100,100,&m,3));assert(!mfw_motion_render(&a,c,99,100,&m,3.1));assert(!a.active);
 mfw_motion_finish(&a);mfw_motion_finish(&a);cairo_destroy(c);cairo_surface_destroy(s);
 return 0;
}

