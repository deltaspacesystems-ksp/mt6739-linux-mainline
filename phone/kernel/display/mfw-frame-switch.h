/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_FRAME_SWITCH_H
#define MFW_FRAME_SWITCH_H
/* Caller serializes request/EOF/cancel and owns/pins every buffer. No MMIO.
 * Two additional EOFs after APPLY are a conservative bringup hold, NOT proof
 * of hardware latch semantics or a final KMS presentation timestamp. */
#ifdef __KERNEL__
#include <linux/errno.h>
#include <linux/types.h>
#else
#include <errno.h>
#include <stdint.h>
typedef uint32_t u32;
#endif
struct mfw_frame_switch { u32 target; unsigned pending, settling; };
enum mfw_switch_event { MFW_SWITCH_NONE, MFW_SWITCH_APPLY, MFW_SWITCH_COMPLETE };
static inline int mfw_switch_request(struct mfw_frame_switch *s,u32 target){
 if(s->pending||s->settling)return -EBUSY;
 if(!target||(target&4095))return -EINVAL;
 s->target=target;s->pending=1;return 0;
}
static inline enum mfw_switch_event mfw_switch_eof(struct mfw_frame_switch *s){
 if(s->pending){s->pending=0;s->settling=2;return MFW_SWITCH_APPLY;}
 if(s->settling&&!--s->settling)return MFW_SWITCH_COMPLETE;
 return MFW_SWITCH_NONE;
}
static inline void mfw_switch_cancel(struct mfw_frame_switch *s){*s=(struct mfw_frame_switch){0};}
#endif
