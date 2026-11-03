/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "pds/pvr_pds.h"
static void persistent(const struct pvr_device_info *dev){
 struct pvr_pds_stream_out_terminate_program s={.pds_persistent_temp_size_to_store=32,.dev_address_for_storing_persistent_temp=UINT64_C(0xa800020000)};
 struct pvr_pds_stream_out_init_program l={.num_buffers=1,.pds_buffer_data_size={32},.dev_address_for_buffer_data={UINT64_C(0xa800020000)}};
 pvr_pds_generate_stream_out_terminate_program(&s,NULL,PDS_GENERATE_SIZES,dev);unsigned sd=s.stream_out_terminate_pds_data_size,sc=s.stream_out_terminate_pds_code_size;
 pvr_pds_generate_stream_out_init_program(&l,NULL,false,PDS_GENERATE_SIZES,dev);unsigned ld=l.stream_out_init_pds_data_size,lc=l.stream_out_init_pds_code_size;
 assert(sd+sc<=64&&ld+lc<=64);_Alignas(16) uint32_t a[68]={0},b[68]={0};a[3]=b[3]=a[4+sd+sc]=b[4+ld+lc]=0xdeadbeef;
 uint32_t *ac=pvr_pds_generate_stream_out_terminate_program(&s,a+4,PDS_GENERATE_DATA_SEGMENT,dev);assert(ac==a+4+sd);uint32_t *ae=pvr_pds_generate_stream_out_terminate_program(&s,ac,PDS_GENERATE_CODE_SEGMENT,dev);assert(ae==ac+sc);
 uint32_t *bc=pvr_pds_generate_stream_out_init_program(&l,b+4,false,PDS_GENERATE_DATA_SEGMENT,dev);assert(bc==b+4+ld);uint32_t *be=pvr_pds_generate_stream_out_init_program(&l,bc,false,PDS_GENERATE_CODE_SEGMENT,dev);assert(be==bc+lc);
 assert(a[3]==0xdeadbeef&&b[3]==0xdeadbeef&&a[4+sd+sc]==0xdeadbeef&&b[4+ld+lc]==0xdeadbeef);
 printf("PT128_PDS_CANDIDATE_PASS store data%u/code%u load data%u/code%u DWORDS\n",sd,sc,ld,lc);
}
int main(void){struct pvr_device_info dev={0};persistent(&dev);
 for(unsigned load=0;load<2;load++){
  struct pvr_pds_shared_storing_program p={.cc_enable=false,.doutw_control={.dest_store=PDS_UNIFIED_STORE,.num_const64=2,.doutw_data={UINT64_C(0xa800030000),UINT64_C(0xa800030030)}}};
  pvr_pds_setup_doutu(&p.usc_task.usc_task_control,load?0x11600:0x11200,10,0,false);
  _Alignas(16) uint32_t data[36]={0},code[36]={0};data[3]=code[3]=0xdeadbeef;
  pvr_pds_generate_shared_storing_program(&p,data+4,PDS_GENERATE_DATA_SEGMENT,&dev);unsigned d=p.data_size;
  uint32_t *end=pvr_pds_generate_shared_storing_program(&p,code+4,PDS_GENERATE_CODE_SEGMENT,&dev);
  assert(d==16&&p.code_size==3&&end==code+7&&data[3]==0xdeadbeef&&code[3]==0xdeadbeef&&data[20]==0&&code[7]==0);
  printf("TA_SR_PDS_CANDIDATE_PASS load%u data16/code3 DWORDS no compute barrier/cc\n",load);
 }
}
