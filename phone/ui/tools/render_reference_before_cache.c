#include "ui.h"
#include <pango/pangocairo.h>
#include <math.h>
#include <stdio.h>

static void rgba(cairo_t*c,double r,double g,double b,double a){cairo_set_source_rgba(c,r,g,b,a);}
static void rr(cairo_t*c,double x,double y,double w,double h,double r){
    const double p=3.141592653589793;
    cairo_new_sub_path(c);cairo_arc(c,x+w-r,y+r,r,-p/2,0);cairo_arc(c,x+w-r,y+h-r,r,0,p/2);
    cairo_arc(c,x+r,y+h-r,r,p/2,p);cairo_arc(c,x+r,y+r,r,p,p*1.5);cairo_close_path(c);
}
static void text(cairo_t*c,const char*s,double x,double y,double size,bool bold,int width,bool center){
    PangoLayout*l=pango_cairo_create_layout(c);PangoFontDescription*f=pango_font_description_new();
    pango_font_description_set_family(f,"SF Pro Text, Inter, DejaVu Sans");
    pango_font_description_set_absolute_size(f,size*PANGO_SCALE);
    pango_font_description_set_weight(f,bold?PANGO_WEIGHT_SEMIBOLD:PANGO_WEIGHT_NORMAL);
    pango_layout_set_font_description(l,f);pango_layout_set_text(l,s,-1);
    pango_layout_set_single_paragraph_mode(l,true);
    if(width>0){pango_layout_set_width(l,width*PANGO_SCALE);pango_layout_set_ellipsize(l,PANGO_ELLIPSIZE_END);}
    if(center)pango_layout_set_alignment(l,PANGO_ALIGN_CENTER);
    cairo_move_to(c,x,y);pango_cairo_show_layout(c,l);pango_font_description_free(f);g_object_unref(l);
}
static void glass(cairo_t*c,double x,double y,double w,double h,double r){
    rr(c,x,y,w,h,r);rgba(c,.11,.14,.19,.88);cairo_fill_preserve(c);
    rgba(c,1,1,1,.16);cairo_set_line_width(c,.7);cairo_stroke(c);
}
static void wallpaper(cairo_t*c,double w,double h){
    cairo_pattern_t*p=cairo_pattern_create_linear(0,0,w,h);
    cairo_pattern_add_color_stop_rgb(p,0,.055,.09,.14);cairo_pattern_add_color_stop_rgb(p,.6,.12,.19,.27);
    cairo_pattern_add_color_stop_rgb(p,1,.095,.13,.20);cairo_set_source(c,p);cairo_paint(c);cairo_pattern_destroy(p);
}
static void status(cairo_t*c,double w,const struct mfw_model*m){
    rgba(c,.02,.03,.05,.22);cairo_rectangle(c,0,0,w,28);cairo_fill(c);
    rgba(c,1,1,1,.95);text(c,m->clock,18,5,14,true,90,false);
    /* Existing final 4.4 geometry, rendered smaller: ICON_H18, body .62H. */
    double u=18*.62,gap=24*u/43,bw=2.166*u,cw=65*u/43,ww=57*u/43;
    double bx=w-18-bw,wx=bx-gap-ww,cx=wx-gap-cw,y=7+(18-u)/2;
    rgba(c,1,1,1,m->demo?1:.28);mfw_icon(c,ICON_CELLULAR,cx,5,cw,18);
    if(m->wifi){rgba(c,1,1,1,.95);mfw_icon(c,ICON_WIFI,wx,5,ww,18);}
    rr(c,bx,y,1.98*u,u,.27*u);rgba(c,1,1,1,.35);cairo_fill(c);
    rr(c,bx+2.05*u,y+.335*u,.116*u,.33*u,.05*u);rgba(c,1,1,1,.46);cairo_fill(c);
    if(m->capacity>=0){cairo_save(c);rr(c,bx,y,1.98*u,u,.27*u);cairo_clip(c);
        rgba(c,1,1,1,1);cairo_rectangle(c,bx,y,1.98*u*m->capacity/100,u);cairo_fill(c);cairo_restore(c);
    }else{rgba(c,1,1,1,.9);text(c,"â€“",bx,y-1,9,true,(int)(1.98*u),true);}
    if(m->charging){rgba(c,.3,1,.57,1);cairo_move_to(c,bx+u*1.1,y-1);cairo_line_to(c,bx+u*.6,y+u*.6);
        cairo_line_to(c,bx+u,y+u*.6);cairo_line_to(c,bx+u*.8,y+u+1);
        cairo_line_to(c,bx+u*1.4,y+u*.35);cairo_line_to(c,bx+u,y+u*.35);cairo_close_path(c);cairo_fill(c);}
}
static void folder(cairo_t*c,double x,double y,double size){
    rgba(c,.48,.70,.89,1);rr(c,x,y+size*.23,size,size*.65,3);cairo_fill(c);
    rgba(c,.60,.78,.94,1);rr(c,x,y+size*.12,size*.5,size*.25,2);cairo_fill(c);
}
static void terminal(cairo_t*c,double x,double y,double size){
    rr(c,x,y,size,size,5);rgba(c,.05,.065,.09,1);cairo_fill_preserve(c);rgba(c,.8,.85,.9,.6);cairo_set_line_width(c,.7);cairo_stroke(c);
    rgba(c,.92,.96,1,1);text(c,">_",x+3,y+size*.20,size*.40,true,(int)size-6,true);
}
static void shortcut(cairo_t*c,double x,double y,int kind,const char*label){
    if(kind==0)folder(c,x+13,y,30);else if(kind==1)terminal(c,x+13,y,30);
    else{rgba(c,.9,.94,1,1);mfw_icon(c,ICON_GEAR,x+13,y,30,30);}
    rgba(c,1,1,1,.95);text(c,label,x-5,y+38,11,false,66,true);
}
static void desktop(cairo_t*c,double w,double h){
    shortcut(c,19,59,0,"Files");shortcut(c,19,143,1,"Terminal");shortcut(c,19,227,2,"Settings");
    double dx=(w-206)/2;glass(c,dx,h-71,206,47,12);
    folder(c,dx+17,h-60,26);terminal(c,dx+65,h-60,26);
    rgba(c,.93,.96,1,1);mfw_icon(c,ICON_GEAR,dx+113,h-60,26,26);
    cairo_rectangle(c,dx+163,h-58,19,16);cairo_set_line_width(c,1.5);cairo_stroke(c);
    cairo_rectangle(c,dx+168,h-53,19,16);cairo_stroke(c);
}
static void settings_content(cairo_t*c,double x,double y,double w,double h,const struct mfw_model*m){
    glass(c,x,y,w,h,12);rgba(c,1,1,1,.95);text(c,"Settings",x+20,y+18,20,true,(int)w-40,false);
    char v[96];const char*labels[]={"Battery","Temperature","Voltage","Processor"};
    for(int i=0;i<4;i++){
        double yy=y+65+i*40;if(yy+35>y+h)break;
        rgba(c,1,1,1,.055);rr(c,x+12,yy,w-24,36,7);cairo_fill(c);
        rgba(c,.93,.95,.98,1);text(c,labels[i],x+23,yy+10,12,false,(int)(w*.45)-20,false);
        if(i==0)snprintf(v,sizeof v,"%s",m->battery_status);
        if(i==1){if(m->temp_deci_c>=0)snprintf(v,sizeof v,"%.1f Â°C",m->temp_deci_c/10.0);else snprintf(v,sizeof v,"â€”");}
        if(i==2){if(m->voltage_uv>0)snprintf(v,sizeof v,"%.3f V",m->voltage_uv/1000000.0);else snprintf(v,sizeof v,"â€”");}
        if(i==3){if(m->cpu_hz)snprintf(v,sizeof v,"%.0f MHz",m->cpu_hz/1000000.0);else snprintf(v,sizeof v,"â€”");}
        rgba(c,.68,.73,.80,1);text(c,v,x+w*.53,yy+10,12,false,(int)(w*.47)-24,true);
    }
}
static void switcher(cairo_t*c,double w,double h,const struct mfw_model*m){
    rgba(c,.015,.025,.04,.3);cairo_paint(c);
    double cw=fmin(480,w-40),x=(w-cw)/2;
    rgba(c,1,1,1,.9);text(c,"Open windows",x+4,55,16,true,(int)cw,false);
    if(!m->n_windows){text(c,"No open windows",x+4,100,12,false,(int)cw,false);return;}
    for(unsigned i=0;i<m->n_windows;i++){
        double y=96+i*58;if(y+52>h-35)break;
        glass(c,x,y,cw,50,10);rgba(c,.94,.96,1,1);
        text(c,m->windows[i].title[0]?m->windows[i].title:m->windows[i].app_id,x+15,y+10,13,true,(int)cw-90,false);
        rgba(c,.65,.72,.83,1);text(c,m->windows[i].minimized?"Minimized":m->windows[i].fullscreen?"Fullscreen":m->windows[i].maximized?"Maximized":"Window",x+15,y+29,10,false,(int)cw-90,false);
        text(c,"Ã—",x+cw-37,y+12,18,false,26,true);
    }
}
static void control(cairo_t*c,double w,double h){
    double panel=fmin(272,w-32),x=w-panel-16,y=43;
    glass(c,x,y,panel,fmin(339,h-70),16);
    rgba(c,.9,.94,1,1);text(c,"Control Center",x+17,y+16,14,true,(int)panel-34,false);
    enum mfw_icon icons[]={ICON_CELLULAR,ICON_WIFI,ICON_GEAR};
    for(int i=0;i<3;i++){double xx=x+16+i*(panel-32)/3;
        rr(c,xx,y+53,68,54,12);rgba(c,1,1,1,.08);cairo_fill(c);rgba(c,.9,.94,1,1);mfw_icon(c,icons[i],xx+23,y+65,24,28);}
    const char*labels[]={"Brightness","Volume"};
    for(int i=0;i<2;i++){double yy=y+133+i*64;rgba(c,.8,.85,.92,1);text(c,labels[i],x+18,yy,11,false,(int)panel-36,false);
        rr(c,x+18,yy+24,panel-36,5,2.5);rgba(c,1,1,1,.15);cairo_fill(c);
        rr(c,x+18,yy+24,(panel-36)*(.65-i*.25),5,2.5);rgba(c,.8,.85,.92,1);cairo_fill(c);}
    enum mfw_icon tools[]={ICON_TORCH,ICON_CLOCK,ICON_CAMERA};
    for(int i=0;i<3;i++){double xx=x+16+i*(panel-32)/3;rr(c,xx,y+265,68,54,12);rgba(c,1,1,1,.08);cairo_fill(c);
        rgba(c,.9,.94,1,1);mfw_icon(c,tools[i],xx+23,y+277,24,28);}
}
void mfw_render(cairo_t*c,int width,int height,const struct mfw_model*m){
    double scale=m->scale>0?m->scale:1,w=width/scale,h=height/scale;
    cairo_save(c);cairo_scale(c,scale,scale);wallpaper(c,w,h);
    if(m->page==PAGE_DESKTOP)desktop(c,w,h);
    else if(m->page==PAGE_SWITCHER)switcher(c,w,h,m);
    else if(m->page==PAGE_CONTROL){desktop(c,w,h);control(c,w,h);}
    else settings_content(c,12,40,w-24,h-66,m);
    status(c,w,m);rr(c,(w-88)/2,h-8,88,3,1.5);rgba(c,1,1,1,.86);cairo_fill(c);
    cairo_restore(c);
}
