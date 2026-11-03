/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "pds/pvr_pds.h"
int main(void){
 for(unsigned variant=0;variant<2;variant++){
 struct pvr_device_info dev={0};
 dev.features.has_slc_mcu_cache_controls=variant;
 printf("CANDIDATE slc_mcu_cache_controls=%u\n",variant); /* offline feature-neutral candidate, not target profile */
 struct pvr_pds_compute_shader_program p;
 pvr_pds_compute_shader_program_init(&p);
 p.local_input_regs[0]=0;p.kick_usc=true;
 pvr_pds_setup_doutu(&p.usc_task_control,0x100,5,0,false);
 uint32_t buffer[1026];for(unsigned i=0;i<1026;i++)buffer[i]=0xdeadbeef;
 pvr_pds_compute_shader(&p,NULL,PDS_GENERATE_SIZES,&dev);
 assert(p.data_size && p.code_size && p.data_size+p.code_size<1024);
 assert((p.code_size%4)==0);
 unsigned data=p.data_size,code_bytes=p.code_size,code=code_bytes/4;
 unsigned code_offset=(data+3)&~3u;
 pvr_pds_compute_shader(&p,buffer+1,PDS_GENERATE_DATA_SEGMENT,&dev);
 assert(p.data_size==data&&p.code_size==code_bytes);
 uint32_t *end=pvr_pds_compute_shader(&p,buffer+1+code_offset,PDS_GENERATE_CODE_SEGMENT,&dev);
 assert(end==buffer+1+code_offset+code);
 assert(p.data_size==data&&p.code_size==code_bytes);
 assert(buffer[0]==0xdeadbeef&&buffer[1+code_offset+code]==0xdeadbeef);
 printf("OFFLINE_PDS data_dwords=%u code_bytes=%u code_dwords=%u temps_used=%u\n",data,code_bytes,code,p.temps_used);
 for(unsigned i=0;i<data;i++)printf("DATA %u %08x\n",i,buffer[1+i]);
 for(unsigned i=0;i<code;i++)printf("CODE %u %08x\n",i,buffer[1+code_offset+i]);
 puts("PASS generator sizes/data/code and end guards; NOT hardware validated");
 }
}
