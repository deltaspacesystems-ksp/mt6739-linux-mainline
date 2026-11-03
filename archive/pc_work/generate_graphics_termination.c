/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#define __pvr_address_type uint64_t
#define __pvr_get_address(a) (a)
#define __pvr_make_address(a) (a)
#include "csbgen/rogue/vdm.h"
#include "csbgen/rogue/ppp.h"
static void ref(uint32_t out[2],uint64_t addr,unsigned count){struct ROGUE_VDMCTRL_PPP_STATE0 a={ROGUE_VDMCTRL_PPP_STATE0_header,.word_count=count,.addrmsb=addr};ROGUE_VDMCTRL_PPP_STATE0_pack(out,&a);struct ROGUE_VDMCTRL_PPP_STATE1 b={.addrlsb=addr};ROGUE_VDMCTRL_PPP_STATE1_pack(out+1,&b);}
int main(void){uint32_t ppp[3]={0},ref_main[2]={0},term[3]={0};
 struct ROGUE_TA_STATE_HEADER h={.pres_terminate=true};ROGUE_TA_STATE_HEADER_pack(ppp,&h);struct ROGUE_TA_STATE_TERMINATE0 a={.clip_right=1,.clip_bottom=1};ROGUE_TA_STATE_TERMINATE0_pack(ppp+1,&a);struct ROGUE_TA_STATE_TERMINATE1 b={0};ROGUE_TA_STATE_TERMINATE1_pack(ppp+2,&b);
 assert(ppp[0]==0x02000000&&ppp[1]==0x00040001&&ppp[2]==0);
 ref(ref_main,UINT64_C(0x65c0005000),25);ref(term,UINT64_C(0x65c0005100),3);struct ROGUE_VDMCTRL_STREAM_TERMINATE t={ROGUE_VDMCTRL_STREAM_TERMINATE_header};ROGUE_VDMCTRL_STREAM_TERMINATE_pack(term+2,&t);
 FILE *f=fopen("/mnt/d/y5_linux_work/out/graphics-termination.bin","wb");assert(f);assert(fwrite(ref_main,4,2,f)==2&&fwrite(ppp,4,3,f)==3&&fwrite(term,4,3,f)==3);assert(!fclose(f));puts("GRAPHICS_TERMINATION_CANDIDATE_PASS PPP references/32pixel terminate blocks; offline only");
}
