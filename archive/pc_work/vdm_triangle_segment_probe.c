/* SPDX-License-Identifier: MIT */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define __pvr_address_type uint64_t
#define __pvr_get_address(a) (a)
#define __pvr_make_address(a) (a)
#include "csbgen/rogue/vdm.h"
int main(void){
 uint32_t s[10]={0};s[0]=s[9]=0xdeadbeef;uint32_t *p=s+1;
 struct ROGUE_VDMCTRL_VDM_STATE0 v0={ROGUE_VDMCTRL_VDM_STATE0_header,.vs_data_addr_present=true,.vs_other_present=true,.cam_size=31,.uvs_scratch_size_select=ROGUE_VDMCTRL_UVS_SCRATCH_SIZE_SELECT_FIVE,.flatshade_control=ROGUE_VDMCTRL_FLATSHADE_CONTROL_VERTEX_0};ROGUE_VDMCTRL_VDM_STATE0_pack(p++,&v0);
 struct ROGUE_VDMCTRL_VDM_STATE2 v2={.vs_pds_data_base_addr=0x10000};ROGUE_VDMCTRL_VDM_STATE2_pack(p++,&v2);
 struct ROGUE_VDMCTRL_VDM_STATE3 v3={.vs_pds_code_base_addr=0x10010};ROGUE_VDMCTRL_VDM_STATE3_pack(p++,&v3);
 struct ROGUE_VDMCTRL_VDM_STATE4 v4={.vs_output_size=5};ROGUE_VDMCTRL_VDM_STATE4_pack(p++,&v4);
 struct ROGUE_VDMCTRL_VDM_STATE5 v5={.vs_max_instances=16,.vs_usc_unified_size=1,.vs_pds_temp_size=1,.vs_pds_data_size=1};ROGUE_VDMCTRL_VDM_STATE5_pack(p++,&v5);
 struct ROGUE_VDMCTRL_INDEX_LIST0 i0={ROGUE_VDMCTRL_INDEX_LIST0_header,.index_count_present=true,.primitive_topology=ROGUE_VDMCTRL_PRIMITIVE_TOPOLOGY_TRI_LIST};ROGUE_VDMCTRL_INDEX_LIST0_pack(p++,&i0);
 struct ROGUE_VDMCTRL_INDEX_LIST2 i2={.index_count=3};ROGUE_VDMCTRL_INDEX_LIST2_pack(p++,&i2);
 struct ROGUE_VDMCTRL_STREAM_TERMINATE t={ROGUE_VDMCTRL_STREAM_TERMINATE_header};ROGUE_VDMCTRL_STREAM_TERMINATE_pack(p++,&t);
 assert(p==s+9&&s[0]==0xdeadbeef&&s[9]==0xdeadbeef);
 const uint32_t expected[8]={0x4c000f80,0x10000,0x10010,5,0x20000441,0x68000600,3,0xc0000000};assert(!memcmp(s+1,expected,sizeof(expected)));
 puts("VDM_TRIANGLE_SEGMENT_PACK_PASS 8DWORDS; incomplete stream, PPP/state/sync absent; NOT submit-ready");
 for(unsigned n=1;n<9;n++){printf("%08x ",s[n]);}puts("");
}

