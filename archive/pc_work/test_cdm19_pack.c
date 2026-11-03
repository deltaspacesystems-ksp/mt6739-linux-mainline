#include <assert.h>
#include <stdint.h>
#define __IMG_EXPLICIT_INCLUDE_HWDEFS
#include "rgx_cr_defs_km.h"
#include "mfw_cdm19_pack.h"
_Static_assert(MFW_CDM19_COMMON_SHIFT==RGX_CR_CDM_CONTEXT_PDS1_COMMON_SIZE_SHIFT,"exact public field shift");
_Static_assert(MFW_CDM19_COMMON_MASK==(uint32_t)~RGX_CR_CDM_CONTEXT_PDS1_COMMON_SIZE_CLRMSK,"exact public field mask");
_Static_assert(RGX_CR_CDM_CONTEXT_PDS0_MASKFULL==UINT64_C(0xfffffff0fffffff0),"PDS0 public fullmask");
_Static_assert(RGX_CR_CDM_CONTEXT_PDS1_UNIFIED_SIZE_SHIFT==21,"unified field shift");
_Static_assert(RGX_CR_CDM_CONTEXT_PDS1_DATA_SIZE_SHIFT==1,"data field shift");
_Static_assert((RGX_CR_CDM_CONTEXT_PDS1_PDS_SEQ_DEP_EN|RGX_CR_CDM_CONTEXT_PDS1_TARGET_EN|RGX_CR_CDM_CONTEXT_PDS1_FENCE_EN)==UINT32_C(0x28000001),"store flags");
_Static_assert(RGX_CR_CDM_CONTEXT_PDS1_COMMON_SHARED_EN==UINT32_C(0x100000),"load flag");
int main(void){
 uint32_t x=0;for(uint32_t n=0;n<=8176;n++){assert(mfw_cdm19_common_field(n,&x));assert(x==((((n*4+63)>>6)<<11)&0xff800));assert(!(x&~MFW_CDM19_COMMON_MASK));}
 assert(mfw_cdm19_common_field(1,&x)&&x==0x800);assert(mfw_cdm19_common_field(16,&x)&&x==0x800);assert(mfw_cdm19_common_field(17,&x)&&x==0x1000);
 assert(mfw_cdm19_common_field(0x800,&x)&&x==0x40000);
 x=123;assert(!mfw_cdm19_common_field(8177,&x)&&x==123);assert(!mfw_cdm19_common_field(UINT32_MAX,&x));assert(!mfw_cdm19_common_field(0,0));
 assert(mfw_cdm19_merge_common(0x28000001,17,&x)&&x==0x28001001);assert(!mfw_cdm19_merge_common(0x28000801,17,&x));
 uint64_t p=0;assert(mfw_cdm19_pds0_offsets(0x12340,0x56780,&p)&&p==UINT64_C(0x0005678000012340));assert(!(p&~RGX_CR_CDM_CONTEXT_PDS0_MASKFULL));
 assert(mfw_cdm19_pds0_offsets(0xfffffff0,0xfffffff0,&p)&&p==RGX_CR_CDM_CONTEXT_PDS0_MASKFULL);
 assert(!mfw_cdm19_pds0_offsets(1,16,&p));assert(!mfw_cdm19_pds0_offsets(16,1,&p));assert(!mfw_cdm19_pds0_offsets(UINT64_C(1)<<32,16,&p));assert(!mfw_cdm19_pds0_offsets(16,UINT64_C(1)<<32,&p));assert(!mfw_cdm19_pds0_offsets(0,0,0));
 for(unsigned u=0;u<=63;u++)for(unsigned d=0;d<=252;d++){
  uint32_t out=0;assert(mfw_cdm19_pds1_control(MFW_CDM19_STORE,u,d,&out));
  assert(((out&~RGX_CR_CDM_CONTEXT_PDS1_UNIFIED_SIZE_CLRMSK)>>21)==u);
  assert(((out&~RGX_CR_CDM_CONTEXT_PDS1_DATA_SIZE_CLRMSK)>>1)==(d+3)/4);
  assert((out&~(UINT32_C(0x07e00000)|UINT32_C(0x7e)))==UINT32_C(0x28000001));
  assert(mfw_cdm19_pds1_control(MFW_CDM19_LOAD,u,d,&out));
  assert((out&~(UINT32_C(0x07e00000)|UINT32_C(0x7e)))==UINT32_C(0x100000));
 }
 assert(mfw_cdm19_pds1_control(MFW_CDM19_FENCE_TERMINATE,0,5,&x)&&x==UINT32_C(0x28100005));
 x=123;assert(!mfw_cdm19_pds1_control(MFW_CDM19_LOAD,64,0,&x)&&x==123);
 assert(!mfw_cdm19_pds1_control(MFW_CDM19_LOAD,0,253,&x));
 assert(!mfw_cdm19_pds1_control(MFW_CDM19_FENCE_TERMINATE,1,0,&x));
 assert(!mfw_cdm19_pds1_control((enum mfw_cdm19_state_task)99,0,0,&x));
 assert(!mfw_cdm19_pds1_control(MFW_CDM19_LOAD,0,0,0));
 uint64_t base=UINT64_C(0xe000000000);
 assert(mfw_cdm19_heap_offset(base+0x1234,base,4,&x)&&x==0x1234);
 assert(mfw_cdm19_heap_offset(base+0xfffffff0,base,16,&x)&&x==0xfffffff0);
 assert(mfw_cdm19_heap_offset(UINT64_MAX,UINT64_MAX-16,16,&x)&&x==16);
 assert(mfw_cdm19_heap_offset(base,base,4,&x)&&x==0);
 x=123;assert(!mfw_cdm19_heap_offset(base-1,base,4,&x)&&x==123);
 assert(!mfw_cdm19_heap_offset(base+(UINT64_C(1)<<32),base,4,&x));
 assert(!mfw_cdm19_heap_offset(base+3,base,4,&x));
 assert(!mfw_cdm19_heap_offset(base,base,0,&x));
 assert(!mfw_cdm19_heap_offset(base,base,3,&x));
 assert(!mfw_cdm19_heap_offset(base,base,4,0));
 base=UINT64_C(0xba00000000);
 assert(mfw_cdm19_heap_range(base,4,base,0x40000000,4,&x)&&x==0);
 assert(mfw_cdm19_heap_range(base+0x3ffffffc,4,base,0x40000000,4,&x)&&x==0x3ffffffc);
 x=123;assert(!mfw_cdm19_heap_range(base+0x3ffffffc,8,base,0x40000000,4,&x)&&x==123);
 assert(!mfw_cdm19_heap_range(base+0x40000000,4,base,0x40000000,4,&x));
 assert(!mfw_cdm19_heap_range(base,0,base,0x40000000,4,&x));
 assert(!mfw_cdm19_heap_range(base,UINT64_MAX,base,0x40000000,4,&x));
 assert(!mfw_cdm19_heap_range(base,4,base,0,4,&x));
 assert(!mfw_cdm19_heap_range(base+0xfffffffc,8,base,UINT64_MAX,4,&x));
 assert(!mfw_cdm19_heap_range(base,4,base,0x40000000,4,0));
 return 0;
}
