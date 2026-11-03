/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pds/pvr_pds.h"
#define __pvr_address_type uint64_t
#define __pvr_get_address(a) (a)
#define __pvr_make_address(a) (a)
#include "csbgen/rogue/cdm.h"
static uint32_t image[1024];
static void compute(struct pvr_pds_compute_shader_program *p,unsigned off,const struct pvr_device_info *dev){
 pvr_pds_compute_shader(p,NULL,PDS_GENERATE_SIZES,dev);assert(!(p->code_size%4));unsigned data=p->data_size,code=p->code_size/4,co=(data+3)&~3u;
 assert((co+code)*4<=256&&off%4==0);pvr_pds_compute_shader(p,image+off,PDS_GENERATE_DATA_SEGMENT,dev);uint32_t *end=pvr_pds_compute_shader(p,image+off+co,PDS_GENERATE_CODE_SEGMENT,dev);assert(end==image+off+co+code);
 printf("PDS object=%u data_dw=%u code_dw=%u code_byte_offset=%u temps=%u\n",off*4,data,code,(off+co)*4,p->temps_used);
}
static void context(int load,const struct pvr_device_info *dev){
 unsigned off=load?0x200/4:0x100/4;uint64_t state=UINT64_C(0x65c0003000);
 struct pvr_pds_shared_storing_program p={.cc_enable=true,.doutw_control={.dest_store=PDS_UNIFIED_STORE,.num_const64=2,.doutw_data={state,state+48}}};
 pvr_pds_setup_doutu(&p.usc_task.usc_task_control,load?0x10600:0x10200,10,0,false);
 pvr_pds_generate_shared_storing_program(&p,image+off,PDS_GENERATE_DATA_SEGMENT,dev);assert(p.data_size==16);
 uint32_t *end=pvr_pds_generate_compute_barrier_conditional(image+off+16,PDS_GENERATE_CODE_SEGMENT);end=pvr_pds_generate_shared_storing_program(&p,end,PDS_GENERATE_CODE_SEGMENT,dev);assert(end==image+off+22);
}
static void kernel(uint32_t *s,unsigned data,unsigned code,unsigned unified,unsigned temp,unsigned fence){
 struct ROGUE_CDMCTRL_KERNEL0 k0={ROGUE_CDMCTRL_KERNEL0_header,.usc_unified_size=unified,.pds_temp_size=temp,.pds_data_size=1,.usc_target=ROGUE_CDMCTRL_USC_TARGET_ANY,.fence=fence};ROGUE_CDMCTRL_KERNEL0_pack(s,&k0);
 struct ROGUE_CDMCTRL_KERNEL1 k1={.data_addr=data,.sd_type=fence?ROGUE_CDMCTRL_SD_TYPE_PDS:ROGUE_CDMCTRL_SD_TYPE_NONE,.usc_common_shared=fence};ROGUE_CDMCTRL_KERNEL1_pack(s+1,&k1);
 struct ROGUE_CDMCTRL_KERNEL2 k2={.code_addr=code};ROGUE_CDMCTRL_KERNEL2_pack(s+2,&k2);
 s[3]=s[4]=s[5]=0;struct ROGUE_CDMCTRL_KERNEL8 k8={.max_instances=1};ROGUE_CDMCTRL_KERNEL8_pack(s+6,&k8);
}
int main(void){
 struct pvr_device_info dev={0};struct pvr_pds_compute_shader_program mainp;
 pvr_pds_compute_shader_program_init(&mainp);mainp.local_input_regs[0]=0;mainp.kick_usc=true;pvr_pds_setup_doutu(&mainp.usc_task_control,0x10000,4,0,false);compute(&mainp,0,&dev);
 context(0,&dev);context(1,&dev);
 struct pvr_pds_fence_program fp={0};pvr_pds_generate_fence_terminate_program(&fp,image+0x300/4,PDS_GENERATE_DATA_SEGMENT,&dev);assert(fp.data_size==4);uint32_t *end=pvr_pds_generate_fence_terminate_program(&fp,image+0x310/4,PDS_GENERATE_CODE_SEGMENT,&dev);assert(end==image+0x324/4); /* Nested CODE generation resets data_size; snapshot DATA result. */
 struct pvr_pds_compute_shader_program fence;pvr_pds_compute_shader_program_init(&fence);fence.fence=true;fence.clear_pds_barrier=true;compute(&fence,0x400/4,&dev);
 assert(mainp.data_size==3&&mainp.code_size==20&&mainp.temps_used==3);
 assert(fence.data_size<=4&&fence.temps_used<=4);
 uint32_t stream[15]={0};kernel(stream,0x10000,0x10010,1,1,0);unsigned fcode=0x10400+((fence.data_size+3)&~3u)*4;kernel(stream+7,0x10400,fcode,0,0,1);struct ROGUE_CDMCTRL_STREAM_TERMINATE term={ROGUE_CDMCTRL_STREAM_TERMINATE_header};ROGUE_CDMCTRL_STREAM_TERMINATE_pack(stream+14,&term);
 FILE *out=fopen("/mnt/d/y5_linux_work/out/compute-pds-candidate.bin","wb");assert(out&&fwrite(image,1,sizeof(image),out)==sizeof(image)&&!fclose(out));out=fopen("/mnt/d/y5_linux_work/out/compute-stream-candidate.bin","wb");assert(out&&fwrite(stream,1,sizeof(stream),out)==sizeof(stream)&&!fclose(out));
 for(unsigned i=0;i<15;i++)printf("CDM %u %08x\n",i,stream[i]);
 puts("OFFLINE bundle candidate generated; not hardware validated");
}
