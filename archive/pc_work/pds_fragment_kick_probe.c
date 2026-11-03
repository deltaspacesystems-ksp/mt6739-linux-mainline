/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "pds/pvr_pds.h"
int main(void){
 struct pvr_pds_kickusc_program p={0};
 pvr_pds_setup_doutu(&p.usc_task_control,0x10000,9,PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SAMPLE_RATE_INSTANCE,false);
 pvr_pds_kick_usc(&p,NULL,0,false,PDS_GENERATE_SIZES);
 unsigned d=p.data_size,c=p.code_size;assert(d+c<128);
 uint32_t storage[132];for(unsigned i=0;i<132;i++)storage[i]=0xdeadbeef;
 memset(storage+1,0,(d+c)*sizeof(uint32_t));
 uint32_t *end=pvr_pds_kick_usc(&p,storage+1,0,false,PDS_GENERATE_CODEDATA_SEGMENTS);
 assert(p.data_size==d&&p.code_size==c);assert(end==storage+1+d+c);assert(storage[0]==0xdeadbeef&&storage[1+d+c]==0xdeadbeef);
 printf("FRAGMENT_KICK_CANDIDATE_PASS data=%u code=%u DWORDS; no shared metadata load included\n",d,c);
 for(unsigned i=0;i<d+c;i++)printf("%08x%s",storage[1+i],i+1==d+c?"\n":" ");
}

