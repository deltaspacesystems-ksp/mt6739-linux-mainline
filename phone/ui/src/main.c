#include "ui.h"
#include "sampler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv) {
    struct mfw_model m={.capacity=-1,.temp_deci_c=-1,.scale=1.0};
    const char *preview=NULL;int width=720,height=1440,frames=0;bool windowed=false,desktop=false,smoke=false;
    for(int i=1;i<argc;i++) {
        if(!strcmp(argv[i],"--demo"))m.demo=true;
        else if(!strcmp(argv[i],"--desktop"))desktop=true;
        else if(!strcmp(argv[i],"--smoke-actions"))smoke=true;
        else if(!strcmp(argv[i],"--windowed"))windowed=true;
        else if(!strcmp(argv[i],"--preview")&&i+1<argc)preview=argv[++i];
        else if(!strcmp(argv[i],"--width")&&i+1<argc)width=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--height")&&i+1<argc)height=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--frames")&&i+1<argc)frames=atoi(argv[++i]);
        else if(!strcmp(argv[i],"--scale")&&i+1<argc)m.scale=atof(argv[++i]);
        else if(!strcmp(argv[i],"--page")&&i+1<argc) {
            const char *s=argv[++i];
            if(!strcmp(s,"desktop"))m.page=PAGE_DESKTOP;
            else if(!strcmp(s,"switcher"))m.page=PAGE_SWITCHER;
            else if(!strcmp(s,"control"))m.page=PAGE_CONTROL;
            else if(!strcmp(s,"settings"))m.page=PAGE_SETTINGS;
            else {fprintf(stderr,"Unknown page\n");return 2;}
        } else {fprintf(stderr,"Usage: %s [--desktop | --windowed] [--demo] [--scale 1.0] [--page desktop|switcher|control|settings] [--preview file.png --width 720 --height 1440] [--frames N] [--smoke-actions]\n",argv[0]);return 2;}
    }
    if(width<320||height<400||width>4096||height>4096||frames<0||!(m.scale>=.75&&m.scale<=2))return 2;
    mfw_sample(&m);
    if(!preview){
        int err=mfw_sampler_start(&m);
        if(err){fprintf(stderr,"Telemetry worker: %s\n",strerror(err));return 1;}
        int result=desktop?mfw_desktop(&m,frames,smoke):mfw_wayland(&m,windowed,frames);
        mfw_sampler_stop();return result;
    }
    cairo_surface_t *s=cairo_image_surface_create(CAIRO_FORMAT_RGB24,width,height);
    cairo_t *c=cairo_create(s);mfw_render(c,width,height,&m);cairo_destroy(c);
    cairo_status_t status=cairo_surface_write_to_png(s,preview);cairo_surface_destroy(s);
    if(status!=CAIRO_STATUS_SUCCESS){fprintf(stderr,"PNG: %s\n",cairo_status_to_string(status));return 1;}
    return 0;
}
