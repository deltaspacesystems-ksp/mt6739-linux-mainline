/* SPDX-License-Identifier: MIT */
#ifndef MFW_CDM19_PACK_H
#define MFW_CDM19_PACK_H
#include <stdbool.h>
#include <stdint.h>
/* Proven non-TEMPSIZE8 layout only; does not build/submit a firmware packet. */
#define MFW_CDM19_COMMON_SHIFT 11u
#define MFW_CDM19_COMMON_MASK UINT32_C(0x000ff800)
#define MFW_CDM19_COMMON_UNIT_DWORDS 16u
#define MFW_CDM19_COMMON_MAX_DWORDS (511u*16u)
static inline bool mfw_cdm19_common_field(uint32_t dwords,uint32_t *out){
 if(!out||dwords>MFW_CDM19_COMMON_MAX_DWORDS)return false;
 *out=((dwords+15u)/16u)<<MFW_CDM19_COMMON_SHIFT;return true;
}
static inline bool mfw_cdm19_merge_common(uint32_t base,uint32_t dwords,uint32_t *out){
 uint32_t field;if(!out||(base&MFW_CDM19_COMMON_MASK)||!mfw_cdm19_common_field(dwords,&field))return false;
 *out=base|field;return true;
}
/* Addresses are heap-relative32-bit PDS offsets, NOT full GPU virtual addresses. */
static inline bool mfw_cdm19_pds0_offsets(uint64_t code,uint64_t data,uint64_t *out){
 if(!out||code>UINT32_MAX||data>UINT32_MAX||(code&15)||(data&15))return false;
 *out=(data<<32)|code;return true;
}
/* Field packing only; caller must separately validate PDS/USC program and resources.
 * unified_units is an already encoded allocation unit count, NOT a register count
 * or an assertion about physical unified-store depth. */
enum mfw_cdm19_state_task { MFW_CDM19_STORE, MFW_CDM19_LOAD, MFW_CDM19_FENCE_TERMINATE };
static inline bool mfw_cdm19_pds1_control(enum mfw_cdm19_state_task task,
 uint32_t unified_units,uint32_t data_dwords,uint32_t *out){
 uint32_t base;
 if(!out||unified_units>63||data_dwords>252)return false;
 switch(task){
 case MFW_CDM19_STORE:base=UINT32_C(0x28000001);break;
 case MFW_CDM19_LOAD:base=UINT32_C(0x00100000);break;
 case MFW_CDM19_FENCE_TERMINATE:
  if(unified_units)return false;
  base=UINT32_C(0x28100001);break;
 default:return false;
 }
 *out=base|(unified_units<<21)|(((data_dwords+3)/4)<<1);return true;
}
/* Legacy allocation helper subtracts the selected heap base for PDS/USC.
 * This does not validate resource ownership or actual heap bounds. */
static inline bool mfw_cdm19_heap_offset(uint64_t va,uint64_t heap_base,
 uint32_t alignment,uint32_t *out){
 if(!out||!alignment||(alignment&(alignment-1))||va<heap_base)return false;
 uint64_t offset=va-heap_base;
 if(offset>UINT32_MAX||(offset&(alignment-1)))return false;
 *out=(uint32_t)offset;return true;
}
/* Use actual queried heap bounds (BRN65273 heaps are smaller than 4GiB). */
static inline bool mfw_cdm19_heap_range(uint64_t va,uint64_t bytes,
 uint64_t base,uint64_t heap_bytes,uint32_t alignment,uint32_t *out){
 uint32_t offset;
 if(!out||!bytes||!mfw_cdm19_heap_offset(va,base,alignment,&offset))return false;
 if((uint64_t)offset>=heap_bytes||bytes>heap_bytes-offset)return false;
 if(bytes-1>UINT32_MAX-(uint64_t)offset)return false;
 *out=offset;return true;
}
#endif
