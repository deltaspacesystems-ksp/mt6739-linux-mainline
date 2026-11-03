#ifndef MFW_MOTION_H
#define MFW_MOTION_H
#include "ui.h"
struct mfw_motion {cairo_surface_t *from,*to;struct mfw_model last;int w,h;double start;bool initialized,active;};
double mfw_motion_now(void);
void mfw_motion_finish(struct mfw_motion *);
bool mfw_motion_render(struct mfw_motion *,cairo_t *,int,int,const struct mfw_model *,double);
#endif
