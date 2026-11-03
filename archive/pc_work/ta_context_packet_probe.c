/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#define __pvr_address_type uint64_t
#define __pvr_get_address(a) (a)
#define __pvr_make_address(a) (a)
#include "csbgen/rogue/vdm.h"
#include "csbgen/rogue/ppp.h"
struct bank {uint64_t store0,store1,store2,resume0,resume1,resume2;};
struct state {uint64_t vdm,ta;struct bank bank[2];};
_Static_assert(sizeof(struct bank)==48&&sizeof(struct state)==112&&offsetof(struct state,bank)==16,"exact loaded firmware context layout");
static uint64_t sr(unsigned data,unsigned load){
 uint32_t w0=0,w1=0;struct ROGUE_VDMCTRL_PDS_STATE0 a={ROGUE_VDMCTRL_PDS_STATE0_header,.dm_target=ROGUE_VDMCTRL_DM_TARGET_VDM,.usc_target=load?ROGUE_VDMCTRL_USC_TARGET_ALL:ROGUE_VDMCTRL_USC_TARGET_ANY,.usc_unified_size=4,.pds_data_size=4};ROGUE_VDMCTRL_PDS_STATE0_pack(&w0,&a);
 struct ROGUE_VDMCTRL_PDS_STATE1 b={.pds_data_addr=data,.sd_type=ROGUE_VDMCTRL_SD_TYPE_PDS,.sd_next_type=ROGUE_VDMCTRL_SD_TYPE_PDS};ROGUE_VDMCTRL_PDS_STATE1_pack(&w1,&b);
 assert(w0==(load?0x20001004u:0x22001004u));assert(w1==(data|5));return ((uint64_t)w1<<32)|w0;
}
static uint64_t pt(unsigned data){uint32_t w0=0,w1=0;struct ROGUE_TA_STATE_STREAM_OUT1 a={.sync=true,.pds_data_size=2};ROGUE_TA_STATE_STREAM_OUT1_pack(&w0,&a);struct ROGUE_TA_STATE_STREAM_OUT2 b={.pds_data_addr=data};ROGUE_TA_STATE_STREAM_OUT2_pack(&w1,&b);assert(w0==0x420&&w1==data);return ((uint64_t)w1<<32)|w0;}
int main(void){struct state s={.vdm=UINT64_C(0x65c0008000),.ta=UINT64_C(0x65c0009000)};
 for(unsigned i=0;i<2;i++){unsigned base=0x12000+i*0x400;s.bank[i]=(struct bank){.store0=sr(base,0),.store1=base+0x40,.store2=pt(base+0x200),.resume0=sr(base+0x100,1),.resume1=base+0x140,.resume2=pt(base+0x300)};}
 puts("TA_CONTEXT112_CANDIDATE_PACK_PASS 2banks, exactstock taskexpressions; addresses not allocated, no submit");
 const unsigned char *bytes=(const unsigned char*)&s;FILE *f=fopen("/mnt/d/y5_linux_work/out/ta-context112-candidate.bin","wb");assert(f);assert(fwrite(bytes,1,sizeof(s),f)==sizeof(s));assert(!fclose(f));
}
