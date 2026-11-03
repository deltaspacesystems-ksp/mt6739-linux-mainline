#include "gestures.h"
#include <string.h>
#include <math.h>

void mfw_gestures_cancel(struct mfw_gestures *g){memset(g,0,sizeof *g);}
static struct mfw_contact *find(struct mfw_gestures*g,int32_t id){
    for(unsigned i=0;i<10;i++)if(g->contacts[i].used&&g->contacts[i].id==id)return &g->contacts[i];
    return NULL;
}
bool mfw_gestures_down(struct mfw_gestures*g,int32_t id,uint32_t ms,double x,double y,double w,double h){
    if(!g->active){mfw_gestures_cancel(g);g->started=ms;g->width=w;g->height=h;}
    if(find(g,id)||g->count>=10){mfw_gestures_cancel(g);return false;}
    struct mfw_contact*c=&g->contacts[g->count++];
    *c=(struct mfw_contact){.id=id,.used=true,.down=true,.sx=x,.sy=y,.x=x,.y=y};
    g->active++;if(g->active>g->peak)g->peak=g->active;return true;
}
void mfw_gestures_motion(struct mfw_gestures*g,int32_t id,double x,double y){
    struct mfw_contact*c=find(g,id);if(c&&c->down){c->x=x;c->y=y;}
}
struct mfw_gesture_result mfw_gestures_up(struct mfw_gestures*g,int32_t id,uint32_t ms){
    struct mfw_gesture_result r={0};struct mfw_contact*c=find(g,id);
    if(!c||!c->down)return r;
    c->down=false;g->active--;if(g->active)return r;
    double sx=0,sy=0,x=0,y=0;
    for(unsigned i=0;i<g->count;i++){c=&g->contacts[i];sx+=c->sx;sy+=c->sy;x+=c->x;y+=c->y;}
    sx/=g->count;sy/=g->count;x/=g->count;y/=g->count;
    r.x=x;r.y=y;r.fingers=g->peak;
    if(g->peak>=3&&y-sy<-60)r.action=ACTION_OVERVIEW;
    else if(g->peak==2&&y-sy>50)r.action=ACTION_LEAVE_FULLSCREEN;
    else if(g->count==1){
        if(sy>g->height-28&&sy-y>35)r.action=(uint32_t)(ms-g->started)>350?ACTION_OVERVIEW:ACTION_DESKTOP;
        else if(sy<28&&y-sy>35)r.action=sx>g->width*.6?ACTION_CONTROL:ACTION_LEAVE_FULLSCREEN;
        else if(fabs(x-sx)<12&&fabs(y-sy)<12)r.action=ACTION_TAP;
    }
    mfw_gestures_cancel(g);return r;
}
