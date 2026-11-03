/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pds/pvr_pds.h"
_Alignas(16) static uint32_t image[1024];
static void kick(unsigned off,unsigned usc,unsigned temps){struct pvr_pds_kickusc_program p={0};pvr_pds_setup_doutu(&p.usc_task_control,usc,temps,PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SAMPLE_RATE_INSTANCE,false);pvr_pds_kick_usc(&p,image+off/4,0,false,PDS_GENERATE_CODEDATA_SEGMENTS);assert(p.data_size==8&&p.code_size==1);}
int main(void){
 struct pvr_device_info dev={0};struct pvr_pds_vertex_primary_program_input v={.flags=PVR_PDS_VERTEX_FLAGS_VERTEX_ID_REQUIRED,.vertex_id_register=0};pvr_pds_setup_doutu(&v.usc_task_control,0x10000,6,0,false);
 unsigned char entries[4096]={0};struct pvr_pds_info i={.entries=(void*)entries,.entries_size_in_bytes=sizeof(entries)};pvr_pds_generate_vertex_primary_program(&v,image+4,&i,false,&dev);assert(i.data_size_in_dwords==3&&i.code_size_in_dwords==3&&i.entry_count==2);
 unsigned cursor=0;
 for(unsigned n=0;n<i.entry_count;n++){
  const struct pvr_const_map_entry *e=(void*)(entries+cursor);
  if(e->type==PVR_PDS_CONST_MAP_ENTRY_TYPE_LITERAL32){struct pvr_const_map_entry_literal32 x;memcpy(&x,e,sizeof(x));assert(x.const_offset<3);image[x.const_offset]=x.literal_value;cursor+=sizeof(x);}
  else if(e->type==PVR_PDS_CONST_MAP_ENTRY_TYPE_DOUTU_ADDRESS){struct pvr_const_map_entry_doutu_address x;memcpy(&x,e,sizeof(x));assert(x.const_offset+2<=3);memcpy(image+x.const_offset,&x.doutu_control,8);cursor+=sizeof(x);}
  else assert(!"unexpected VS constant map entry");
 }
 assert(cursor==i.entries_written_size_in_bytes&&image[0]==0x10000&&image[1]==0x10&&image[2]==0x80000000);
 struct pvr_pds_pixel_shader_sa_program m={.num_dword_doutw=1,.dword_doutw_value={0x202},.dword_doutw_control={PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTW_SRC1_DEST_COMMON_STORE|PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTW_SRC1_BSIZE_LOWER}};
 pvr_pds_pixel_shader_uniform_texture_data(&m,image+0x100/4,PDS_GENERATE_DATA_SEGMENT,true,&dev);assert(m.data_size==2);pvr_pds_pixel_shader_uniform_texture_code(&m,image+0x110/4,PDS_GENERATE_CODE_SEGMENT);assert(m.code_size==1);
 kick(0x200,0x10400,9);kick(0x400,0x10c00,0);
 struct pvr_pds_event_program e={0};pvr_pds_setup_doutu(&e.task_control,0x10800,2,PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTU_SAMPLE_RATE_INSTANCE,false);pvr_pds_generate_pixel_event(&e,image+0x300/4,PDS_GENERATE_DATA_SEGMENT,&dev);assert(e.data_size==2);pvr_pds_generate_pixel_event(&e,image+0x310/4,PDS_GENERATE_CODE_SEGMENT,&dev);assert(e.code_size==1);
 FILE *f=fopen("/mnt/d/y5_linux_work/out/graphics-main-pds.bin","wb");assert(f);assert(fwrite(image,1,sizeof(image),f)==sizeof(image));assert(!fclose(f));puts("GRAPHICS_MAIN_PDS4096_PASS VS/meta/FS/EOT/sync; offline only");
}
