/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "pds/pvr_pds.h"
_Alignas(16) static uint32_t image[1024];
int main(void){struct pvr_device_info dev={0};
 for(unsigned bank=0;bank<2;bank++){
  unsigned start=bank*0x100;uint64_t pt=UINT64_C(0xa800020000)+bank*128,sr=UINT64_C(0xa800030000)+bank*0x5000;
  struct pvr_pds_stream_out_terminate_program s={.pds_persistent_temp_size_to_store=32,.dev_address_for_storing_persistent_temp=pt};
  uint32_t *p=image+start+0x80;uint32_t *c=pvr_pds_generate_stream_out_terminate_program(&s,p,PDS_GENERATE_DATA_SEGMENT,&dev);assert(c==p+8);uint32_t *end=pvr_pds_generate_stream_out_terminate_program(&s,c,PDS_GENERATE_CODE_SEGMENT,&dev);assert(end==p+13);
  struct pvr_pds_stream_out_init_program l={.num_buffers=1,.pds_buffer_data_size={32},.dev_address_for_buffer_data={pt}};
  p=image+start+0xc0;c=pvr_pds_generate_stream_out_init_program(&l,p,false,PDS_GENERATE_DATA_SEGMENT,&dev);assert(c==p+8);end=pvr_pds_generate_stream_out_init_program(&l,c,false,PDS_GENERATE_CODE_SEGMENT,&dev);assert(end==p+14);
  for(unsigned load=0;load<2;load++){
   struct pvr_pds_shared_storing_program q={.cc_enable=false,.doutw_control={.dest_store=PDS_UNIFIED_STORE,.num_const64=2,.doutw_data={sr,sr+48}}};
   pvr_pds_setup_doutu(&q.usc_task.usc_task_control,load?0x11600:0x11200,10,0,false);p=image+start+load*0x40;
   pvr_pds_generate_shared_storing_program(&q,p,PDS_GENERATE_DATA_SEGMENT,&dev);assert(q.data_size==16);end=pvr_pds_generate_shared_storing_program(&q,p+16,PDS_GENERATE_CODE_SEGMENT,&dev);assert(end==p+19);
  }
 }
 FILE *f=fopen("/mnt/d/y5_linux_work/out/graphics-context-pds.bin","wb");assert(f);assert(fwrite(image,1,sizeof(image),f)==sizeof(image));assert(!fclose(f));puts("GRAPHICS_CONTEXT_PDS4096_PASS 2banks 8programs; offline only");
}
