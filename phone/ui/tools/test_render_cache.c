#include "ui.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
extern void mfw_render_reference(cairo_t*,int,int,const struct mfw_model*);
static cairo_surface_t*draw(int w,int h,int strip,const struct mfw_model*m,bool reference){
    int sh=strip==1?(int)(28*m->scale):strip==2?(int)(24*m->scale):h;
    cairo_surface_t*s=cairo_image_surface_create(CAIRO_FORMAT_RGB24,w,sh);cairo_t*c=cairo_create(s);
    if(strip==2)cairo_translate(c,0,-(h-sh));
    if(reference)mfw_render_reference(c,w,h,m);else mfw_render(c,w,h,m);
    cairo_destroy(c);cairo_surface_flush(s);return s;
}
int main(void){
    struct mfw_model m={.demo=true,.charging=true,.wifi=true,.capacity=73,.scale=1.0,.voltage_uv=4000000,.temp_deci_c=300,.cpu_hz=950000000};
    strcpy(m.clock,"9:41");strcpy(m.battery_status,"Charging");
    const double scales[]={.75,1,1.25};const int widths[]={720,360};unsigned cases=0;
    for(unsigned size=0;size<2;size++)for(unsigned scale=0;scale<3;scale++)for(unsigned page=0;page<4;page++)for(int strip=0;strip<3;strip++){
        m.scale=scales[scale];m.page=(enum mfw_page)page;int w=widths[size],h=size?640:1440;
        cairo_surface_t*a=draw(w,h,strip,&m,true),*b=draw(w,h,strip,&m,false);
        int sh=cairo_image_surface_get_height(a),stride=cairo_image_surface_get_stride(a)/4;
        uint32_t*x=(uint32_t*)cairo_image_surface_get_data(a),*y=(uint32_t*)cairo_image_surface_get_data(b);
        for(int row=0;row<sh;row++)for(int col=0;col<w;col++)if((x[row*stride+col]&0xffffff)!=(y[row*stride+col]&0xffffff)){
            fprintf(stderr,"Pixel mismatch page=%u scale=%.2f size=%dx%d strip=%d at=%d,%d\n",page,m.scale,w,h,strip,col,row);return 1;
        }
        cairo_surface_destroy(a);cairo_surface_destroy(b);cases++;
    }
    printf("PASS: cached/background reference RGB pixel equality in %u page/scale/strip/size cases\n",cases);return 0;
}
