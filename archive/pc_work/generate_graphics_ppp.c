/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define __pvr_address_type uint64_t
#define __pvr_get_address(a) (a)
#define __pvr_make_address(a) (a)
#include "csbgen/rogue/ppp.h"
#define EMIT(T,...) do {struct ROGUE_TA_##T v={__VA_ARGS__};ROGUE_TA_##T##_pack(p++,&v);}while(0)
static uint32_t bits(float f){uint32_t u;memcpy(&u,&f,4);return u;}
int main(void){uint32_t storage[27]={0};storage[0]=storage[26]=0xdeadbeef;uint32_t *p=storage+1;
 EMIT(STATE_HEADER,.pres_ispctl=true,.pres_ispctl_fa=true,.pres_ispctl_fb=true,.pres_pds_state_ptr0=true,.pres_pds_state_ptr1=true,.pres_pds_state_ptr3=true,.pres_region_clip=true,.pres_viewport=true,.pres_wclamp=true,.pres_outselects=true,.pres_varying_word0=true,.pres_varying_word1=true,.pres_varying_word2=true,.pres_ppp_ctrl=true,.pres_stream_out_size=true);
 EMIT(STATE_ISPCTL,.bpres=true);
 EMIT(STATE_ISPA,.objtype=ROGUE_TA_OBJTYPE_TRIANGLE,.passtype=ROGUE_TA_PASSTYPE_PUNCH_THROUGH,.dwritedisable=true,.dcmpmode=ROGUE_TA_CMPMODE_ALWAYS);
 EMIT(STATE_ISPB,.scmpmode=ROGUE_TA_CMPMODE_ALWAYS);
 EMIT(STATE_PDS_SHADERBASE,.addr=0x10200);
 EMIT(STATE_PDS_TEXUNICODEBASE,.addr=0x10110);
 EMIT(STATE_PDS_SIZEINFO1,.pds_uniformsize=1);
 EMIT(STATE_PDS_SIZEINFO2,.usc_sharedsize=1);
 EMIT(STATE_PDS_VARYINGBASE,.addr=0);
 EMIT(STATE_PDS_UNIFORMDATABASE,.addr=0x10100);
 EMIT(REGION_CLIP0,.mode=ROGUE_TA_REGION_CLIP_MODE_OUTSIDE,.right=3);
 EMIT(REGION_CLIP1,.bottom=3);
 *p++=bits(32);*p++=bits(32);*p++=bits(32);*p++=bits(32);*p++=bits(0);*p++=bits(1);
 EMIT(WCLAMP,.val=bits(1.0e-13f));
 EMIT(OUTPUT_SEL,.vtxsize=5,.psprite_size_pres=true);
 EMIT(STATE_VARYING0,0);EMIT(STATE_VARYING1,0);EMIT(STATE_VARYING2,0);
 EMIT(STATE_PPP_CTRL,.cullmode=ROGUE_TA_CULLMODE_NO_CULLING);
 EMIT(STATE_STREAM_OUT0,0);
 assert(p==storage+26&&storage[0]==0xdeadbeef&&storage[26]==0xdeadbeef);assert(storage[20]==0x05040000);
 puts("PPP25_CANDIDATE_PACK_PASS normalVS5outputs/FSdiscard/punchthrough; offline only, regionclip16 target correlation pending");
 FILE *out=fopen("/mnt/d/y5_linux_work/out/graphics-ppp.bin","wb");assert(out);assert(fwrite(storage+1,4,25,out)==25);assert(!fclose(out));
 for(unsigned i=1;i<26;i++){printf("%08x ",storage[i]);}puts("");
}
