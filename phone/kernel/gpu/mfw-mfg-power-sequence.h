/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MFW_MFG_POWER_SEQUENCE_H
#define MFW_MFG_POWER_SEQUENCE_H
/* MT6739 stock-matched sequencing. Caller provides a prepared26MHz clock,
 * verified read-only VCORE voltage, exclusive operation lock and bounded polls.
 * No firmware, voltage, PLL, HWAPM or GPU MMIO here. On failure caller MUST
 * retain the clock if either domain may still be powered; never blind rollback.
 */
#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
typedef uint32_t u32;
#endif
enum mfw_mfg_block { MFW_MFG_SPM, MFW_MFG_INFRA_AO, MFW_MFG_INFRA };
struct mfw_mfg_io {
 void *ctx;
 u32 (*read)(void *,enum mfw_mfg_block,u32);
 void (*write)(void *,enum mfw_mfg_block,u32,u32);
 int (*poll)(void *,enum mfw_mfg_block,u32,u32,u32);
 int (*idle)(void *);
 unsigned stage;
};
static inline void mfw_mfg_update(struct mfw_mfg_io *io,enum mfw_mfg_block b,u32 off,u32 mask,u32 value){
 io->write(io->ctx,b,off,(io->read(io->ctx,b,off)&~mask)|(value&mask));
}
static inline int mfw_mfg_status(struct mfw_mfg_io *io,u32 mask,u32 value){
 int ret=io->poll(io->ctx,MFW_MFG_SPM,0x180,mask,value);
 return ret?ret:io->poll(io->ctx,MFW_MFG_SPM,0x184,mask,value);
}
static inline int mfw_mfg_domain_on(struct mfw_mfg_io *io,u32 off,u32 mask){
 int ret;
 mfw_mfg_update(io,MFW_MFG_SPM,off,4,4);
 mfw_mfg_update(io,MFW_MFG_SPM,off,8,8);
 ret=mfw_mfg_status(io,mask,mask);if(ret)return ret;
 mfw_mfg_update(io,MFW_MFG_SPM,off,16,0);
 mfw_mfg_update(io,MFW_MFG_SPM,off,2,0);
 mfw_mfg_update(io,MFW_MFG_SPM,off,1,1);
 mfw_mfg_update(io,MFW_MFG_SPM,off,32,0);
 return io->poll(io->ctx,MFW_MFG_SPM,off,64,0);
}
static inline int mfw_mfg_domain_off(struct mfw_mfg_io *io,u32 off,u32 mask){
 int ret;
 mfw_mfg_update(io,MFW_MFG_SPM,off,32,32);
 ret=io->poll(io->ctx,MFW_MFG_SPM,off,64,64);if(ret)return ret;
 mfw_mfg_update(io,MFW_MFG_SPM,off,2,2);
 mfw_mfg_update(io,MFW_MFG_SPM,off,16,16);
 mfw_mfg_update(io,MFW_MFG_SPM,off,1,0);
 mfw_mfg_update(io,MFW_MFG_SPM,off,4,0);
 mfw_mfg_update(io,MFW_MFG_SPM,off,8,0);
 return mfw_mfg_status(io,mask,0);
}
static inline int mfw_mfg_power_up(struct mfw_mfg_io *io){
 int ret;io->stage=1;
 ret=mfw_mfg_domain_on(io,0x338,1U<<4);if(ret)return ret;
 io->stage=2;io->write(io->ctx,MFW_MFG_INFRA_AO,0x2a4,0x600000);
 mfw_mfg_update(io,MFW_MFG_INFRA_AO,0x200,2,0);
 mfw_mfg_update(io,MFW_MFG_INFRA_AO,0x200,128,128);
 ret=io->idle(io->ctx);if(ret)return ret;
 io->stage=3;ret=mfw_mfg_domain_on(io,0x33c,1U<<31);if(ret)return ret;
 io->stage=4;return 0;
}
static inline int mfw_mfg_power_down(struct mfw_mfg_io *io){
 int ret;io->stage=5;
 ret=mfw_mfg_domain_off(io,0x33c,1U<<31);if(ret)return ret;
 io->stage=6;mfw_mfg_update(io,MFW_MFG_INFRA_AO,0x200,128,0);
 ret=io->idle(io->ctx);if(ret)return ret;
 mfw_mfg_update(io,MFW_MFG_INFRA_AO,0x200,2,2);
 io->write(io->ctx,MFW_MFG_INFRA_AO,0x2a0,0x600000);
 ret=io->poll(io->ctx,MFW_MFG_INFRA_AO,0x228,0x600000,0x600000);if(ret)return ret;
 io->stage=7;ret=mfw_mfg_domain_off(io,0x338,1U<<4);if(ret)return ret;
 io->stage=8;return 0;
}
#endif
