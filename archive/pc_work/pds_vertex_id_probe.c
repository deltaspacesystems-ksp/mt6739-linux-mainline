/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "pds/pvr_pds.h"
int main(void){
 struct pvr_device_info dev={0};struct pvr_pds_vertex_primary_program_input p={.flags=PVR_PDS_VERTEX_FLAGS_VERTEX_ID_REQUIRED,.vertex_id_register=0};
 pvr_pds_setup_doutu(&p.usc_task_control,0x10000,6,0,false);
 unsigned char entries[4096]={0};struct pvr_pds_info i={.entries=(void*)entries,.entries_size_in_bytes=sizeof(entries)};
 pvr_pds_generate_vertex_primary_program(&p,NULL,&i,false,&dev);
 unsigned code_size=i.code_size_in_dwords;assert(code_size<128);uint32_t code[130]={0};code[0]=code[code_size+1]=0xdeadbeef;
 pvr_pds_generate_vertex_primary_program(&p,code+1,&i,false,&dev);
 assert(i.code_size_in_dwords==code_size&&code[0]==0xdeadbeef&&code[code_size+1]==0xdeadbeef&&i.entries_written_size_in_bytes<=sizeof(entries));
 printf("VS_VERTEX_ID0_CANDIDATE_PASS data=%u code=%u temps=%u entries=%u bytes=%zu; offline only\n",i.data_size_in_dwords,i.code_size_in_dwords,i.temps_required,i.entry_count,i.entries_written_size_in_bytes);
 for(unsigned n=0;n<code_size;n++){printf("%08x ",code[1+n]);}puts("");
 for(size_t n=0;n<i.entries_written_size_in_bytes;n++){printf("%02x",entries[n]);}puts("");
}
