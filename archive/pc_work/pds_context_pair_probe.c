/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "pds/pvr_pds.h"
int main(void){
 for(unsigned load=0;load<2;load++)for(unsigned slc=0;slc<2;slc++){
 struct pvr_device_info dev={0};dev.features.has_slc_mcu_cache_controls=slc;
 uint64_t state=UINT64_C(0xa800010000),usc=0x2000;
 struct pvr_pds_shared_storing_program p={.cc_enable=true,.doutw_control={.dest_store=PDS_UNIFIED_STORE,.num_const64=2,.doutw_data={state,state+48},.last_instruction=false}};
 pvr_pds_setup_doutu(&p.usc_task.usc_task_control,usc,load?20:8,0,false);
 uint32_t data[66]={0},code[66]={0};data[0]=code[0]=0xdeadbeef;
 pvr_pds_generate_shared_storing_program(&p,data+1,PDS_GENERATE_DATA_SEGMENT,&dev);
 uint32_t *end=pvr_pds_generate_compute_barrier_conditional(code+1,PDS_GENERATE_CODE_SEGMENT);
 end=pvr_pds_generate_shared_storing_program(&p,end,PDS_GENERATE_CODE_SEGMENT,&dev);
 assert(p.data_size==16&&p.code_size==3&&end==code+7);
 assert(data[0]==0xdeadbeef&&code[0]==0xdeadbeef&&data[17]==0&&code[7]==0);
 uint32_t expected_data[16]={(uint32_t)(state>>32),(uint32_t)state,2,0,(uint32_t)((state+48)>>32),(uint32_t)(state+48),0x80004002,0,(uint32_t)usc,load?0x28:0x10,0,0,0,0,0,0};
 uint32_t expected_code[6]={0xc1100001,0xdb000000,0xc1080001,0xf0020001,0xf0060201,0xfc000402};
 if(slc==0){assert(!memcmp(data+1,expected_data,sizeof(expected_data)));assert(!memcmp(code+1,expected_code,sizeof(expected_code)));}
 printf("PASS context%s slc=%u data16dword code6dword; stock expressions%s\n",load?"load":"store",slc,slc?" not asserted for alternate SLC branch":" match");
 }
}
