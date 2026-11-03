/* SPDX-License-Identifier: MIT
 * Bounded software renderer timing, no display/input hardware access.
 * Excludes PNG encoding, compositor copy and sensor sampling.
 */
#define _POSIX_C_SOURCE 200809L
#include "ui.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
static uint64_t ns(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return (uint64_t)t.tv_sec*1000000000ULL+t.tv_nsec;}
static void bench(struct mfw_model *m,const char *name,int h,int offset){
 cairo_surface_t *s=cairo_image_surface_create(CAIRO_FORMAT_RGB24,720,h);
 uint64_t total=0,min=UINT64_MAX,max=0;
 for(unsigned i=0;i<6;i++){
  uint64_t start=ns();cairo_t *c=cairo_create(s);cairo_translate(c,0,-offset);
  mfw_render(c,720,1440,m);cairo_destroy(c);cairo_surface_flush(s);
  uint64_t elapsed=ns()-start;
  if(i){total+=elapsed;if(elapsed<min)min=elapsed;if(elapsed>max)max=elapsed;}
 }
 printf("RENDER_BENCH page=%d view=%s avg_us=%llu min_us=%llu max_us=%llu samples=5\n",m->page,name,(unsigned long long)(total/5000),(unsigned long long)(min/1000),(unsigned long long)(max/1000));
 cairo_surface_destroy(s);
}
int main(void){
 struct mfw_model m={.demo=true,.charging=true,.wifi=true,.capacity=70,.voltage_uv=4100000,.temp_deci_c=300,.cpu_hz=950000000,.scale=1.0};
 strcpy(m.clock,"17:45");strcpy(m.battery_status,"Charging");
 for(int page=0;page<4;page++){m.page=(enum mfw_page)page;bench(&m,"full",1440,0);bench(&m,"status_clipped",28,0);bench(&m,"home_clipped",24,1416);}
 return 0;
}
