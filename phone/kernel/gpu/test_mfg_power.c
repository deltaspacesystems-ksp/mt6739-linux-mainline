/* SPDX-License-Identifier: GPL-2.0-only */
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
 for(unsigned fail=1;fail<=7;fail++){init(&m,&io);assert(!mfw_mfg_power_up(&io));m.fail_poll=m.polls+fail;assert(mfw_mfg_power_down(&io)==-ETIMEDOUT);assert(io.stage>=5&&io.stage<8);}
 puts("PASS: parent/core order, SRAM/power acknowledgements and injected timeout stop points; simulation only");return 0;}
