/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_GPU_ID_H
#define MFW_GPU_ID_H
#ifdef __KERNEL__
#include <linux/errno.h>
#include <linux/types.h>
#else
#include <errno.h>
#include <stdint.h>
typedef uint32_t u32;
typedef uint64_t u64;
#endif
struct mfw_gpu_id { unsigned b,v,n,c; };
/* Linux7.3pvr_gpuid_decode_reg layout, no apphint or compiled-BVNC fallback. */
static inline int mfw_gpu_decode_id(u32 core_id,u64 pbvnc,struct mfw_gpu_id *id){
 id->b=(pbvnc>>48)&65535;
 if(id->b){id->v=(pbvnc>>32)&65535;id->n=(pbvnc>>16)&65535;id->c=pbvnc&65535;}
 else {id->b=(pbvnc>>16)&255;id->v=(pbvnc>>8)&255;id->n=(core_id>>8)&255;id->c=core_id&255;}
 return !id->b||id->b==65535||!id->n?-ENODEV:0;
}
#endif
