/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "pds/pvr_pds.h"
#include "pds/pvr_rogue_pds_defs.h"
int main(void){
 struct pvr_device_info dev={0};
 struct pvr_pds_pixel_shader_sa_program p={.num_dword_doutw=1,.dword_doutw_value={0x202},.dword_doutw_control={PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTW_SRC1_DEST_COMMON_STORE|PVR_ROGUE_PDSINST_DOUT_FIELDS_DOUTW_SRC1_BSIZE_LOWER},.kick_usc=false};
 pvr_pds_pixel_shader_uniform_texture_data(&p,NULL,PDS_GENERATE_SIZES,true,&dev);
 unsigned d=p.data_size;
 pvr_pds_pixel_shader_uniform_texture_code(&p,NULL,PDS_GENERATE_SIZES);
 unsigned c=p.code_size;assert(d<128&&c<128);
 _Alignas(16) uint32_t data[132]={0},code[132]={0};data[3]=code[3]=0xdeadbeef;data[4+d]=code[4+c]=0xdeadbeef;
 pvr_pds_pixel_shader_uniform_texture_data(&p,data+4,PDS_GENERATE_DATA_SEGMENT,true,&dev);
 uint32_t *end=pvr_pds_pixel_shader_uniform_texture_code(&p,code+4,PDS_GENERATE_CODE_SEGMENT);
 assert(end==code+4+c&&data[3]==0xdeadbeef&&code[3]==0xdeadbeef&&data[4+d]==0xdeadbeef&&code[4+c]==0xdeadbeef);
 printf("FS_META_SHARED0_CANDIDATE_PASS value202 data=%u code=%u DWORDS; offline only\n",d,c);
 for(unsigned i=0;i<d;i++){printf("%08x ",data[4+i]);}puts("");
 for(unsigned i=0;i<c;i++){printf("%08x ",code[4+i]);}puts("");
}
