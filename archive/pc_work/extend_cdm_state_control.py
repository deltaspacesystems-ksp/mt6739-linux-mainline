from pathlib import Path
p=Path('D:/y5_linux_work/mfw_cdm19_pack.h');s=p.read_text();a='#endif\n';assert s.endswith(a)
code='''/* Field packing only; caller must separately validate PDS/USC program and resources.
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
'''
p.write_text(s[:-len(a)]+code+a,newline='\n')
p=Path('D:/y5_linux_work/test_cdm19_pack.c');s=p.read_text();s=s.replace('int main(void){','''_Static_assert(RGX_CR_CDM_CONTEXT_PDS1_UNIFIED_SIZE_SHIFT==21,"unified field shift");
_Static_assert(RGX_CR_CDM_CONTEXT_PDS1_DATA_SIZE_SHIFT==1,"data field shift");
_Static_assert((RGX_CR_CDM_CONTEXT_PDS1_PDS_SEQ_DEP_EN|RGX_CR_CDM_CONTEXT_PDS1_TARGET_EN|RGX_CR_CDM_CONTEXT_PDS1_FENCE_EN)==UINT32_C(0x28000001),"store flags");
_Static_assert(RGX_CR_CDM_CONTEXT_PDS1_COMMON_SHARED_EN==UINT32_C(0x100000),"load flag");
int main(void){''')
a='return 0;\n}';code='''
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
 return 0;
}''';assert a in s;s=s.replace(a,code);p.write_text(s,newline='\n')
