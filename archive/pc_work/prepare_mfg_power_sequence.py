from pathlib import Path
w=Path(r'D:\y5_linux_work');d=w/'microfw-gpu'
(d/'mfw-mfg-power-sequence.h').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
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
''',newline='\n')
(d/'test_mfg_power.c').write_text('''/* SPDX-License-Identifier: GPL-2.0-only */
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "mfw-mfg-power-sequence.h"
struct mock {u32 r[3][1024];unsigned polls,fail_poll;int idle_fail;unsigned core_started,parent_off;};
static u32 rd(void *ctx,enum mfw_mfg_block b,u32 off){struct mock*m=ctx;assert(off<4096&&!(off&3));return m->r[b][off/4];}
static void wr(void*ctx,enum mfw_mfg_block b,u32 off,u32 value){
 struct mock*m=ctx;m->r[b][off/4]=value;
 if(b==MFW_MFG_SPM&&(off==0x338||off==0x33c)){
  u32 mask=off==0x338?1U<<4:1U<<31;
  if((value&12)==12){
   if(off==0x33c){assert(m->r[0][0x180/4]&(1U<<4));m->core_started++;}
   m->r[0][0x180/4]|=mask;m->r[0][0x184/4]|=mask;
  }else if(!(value&12)){
   if(off==0x338){assert(!(m->r[0][0x180/4]&(1U<<31)));m->parent_off++;}
   m->r[0][0x180/4]&=~mask;m->r[0][0x184/4]&=~mask;
  }
  if(value&32)m->r[b][off/4]|=64;else m->r[b][off/4]&=~64U;
 }
 if(b==MFW_MFG_INFRA_AO&&off==0x2a0)m->r[b][0x228/4]|=value;
 if(b==MFW_MFG_INFRA_AO&&off==0x2a4)m->r[b][0x228/4]&=~value;
}
static int poll(void*ctx,enum mfw_mfg_block b,u32 off,u32 mask,u32 wanted){struct mock*m=ctx;if(++m->polls==m->fail_poll)return -ETIMEDOUT;return (rd(ctx,b,off)&mask)==wanted?0:-ETIMEDOUT;}
static int idle(void*ctx){return ((struct mock*)ctx)->idle_fail?-ETIMEDOUT:0;}
static void init(struct mock*m,struct mfw_mfg_io*io){memset(m,0,sizeof *m);m->r[0][0x338/4]=m->r[0][0x33c/4]=0x72;*io=(struct mfw_mfg_io){.ctx=m,.read=rd,.write=wr,.poll=poll,.idle=idle};}
int main(void){struct mock m;struct mfw_mfg_io io;
 init(&m,&io);assert(!mfw_mfg_power_up(&io)&&io.stage==4);assert((m.r[0][0x180/4]&0x80000010)==0x80000010);
 assert(!mfw_mfg_power_down(&io)&&io.stage==8);assert(!(m.r[0][0x180/4]&0x80000010));assert(m.core_started&&m.parent_off);
 for(unsigned fail=1;fail<=6;fail++){init(&m,&io);m.fail_poll=fail;assert(mfw_mfg_power_up(&io)==-ETIMEDOUT);assert(io.stage<4);}
 init(&m,&io);m.idle_fail=1;assert(mfw_mfg_power_up(&io)==-ETIMEDOUT&&io.stage==2);
 init(&m,&io);assert(!mfw_mfg_power_up(&io));m.fail_poll=m.polls+1;assert(mfw_mfg_power_down(&io)==-ETIMEDOUT&&io.stage==5);assert(m.r[0][0x180/4]&(1U<<4));
 puts("PASS: parent/core order, SRAM/power acknowledgements and injected timeout stop points; simulation only");return 0;}
''',newline='\n')
(w/'test_mfg_power_sequence.sh').write_text('''#!/bin/bash
set -eu
cd /mnt/d/y5_linux_work/microfw-gpu
cc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined test_mfg_power.c -o /tmp/mfw-test-mfg-power
/tmp/mfw-test-mfg-power
''',newline='\n')
