/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "pds/pvr_pds.h"
int main(void){
 struct pvr_device_info dev={0};struct pvr_pds_event_program p={0};
 pvr_pds_setup_doutu(&p.task_control,0x10800,2,PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SAMPLE_RATE_INSTANCE,false);
 pvr_pds_generate_pixel_event(&p,NULL,PDS_GENERATE_SIZES,&dev);
 unsigned d=p.data_size,c=p.code_size;assert(d<32&&c<32);uint32_t data[34]={0},code[34]={0};data[0]=code[0]=data[1+d]=code[1+c]=0xdeadbeef;
 uint32_t *de=pvr_pds_generate_pixel_event(&p,data+1,PDS_GENERATE_DATA_SEGMENT,&dev);uint32_t *ce=pvr_pds_generate_pixel_event(&p,code+1,PDS_GENERATE_CODE_SEGMENT,&dev);
 assert(de==data+1+d&&ce==code+1+c&&data[0]==0xdeadbeef&&code[0]==0xdeadbeef&&data[1+d]==0xdeadbeef&&code[1+c]==0xdeadbeef);
 printf("EOT_PDS_CANDIDATE_PASS data=%u code=%u DWORDS; offline only\n",d,c);for(unsigned i=0;i<d;i++){printf("%08x ",data[1+i]);}puts("");for(unsigned i=0;i<c;i++){printf("%08x ",code[1+i]);}puts("");
}
