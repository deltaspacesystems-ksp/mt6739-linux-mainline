#ifndef MFW_SAMPLER_H
#define MFW_SAMPLER_H
#include "ui.h"
/* One sampler per process. UI thread owns start/read/stop; only telemetry
 * crosses the mutex, never Wayland/Cairo objects or window/gesture state. */
int mfw_sampler_start(const struct mfw_model *initial);
void mfw_sampler_read(struct mfw_model *model);
void mfw_sampler_stop(void);
#endif
