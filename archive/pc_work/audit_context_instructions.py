from pathlib import Path
import re,json
roots={'legacy':Path('/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19'),'mesa':Path('/home/y5/mesa-native/src/imagination')}
ids=['60084','61450','63027','65273','63553'];out={x:{k:[] for k in roots} for x in ids}
for kind,root in roots.items():
 for p in root.rglob('*'):
  if p.suffix not in ['.c','.h','.py','.xml']:continue
  for n,line in enumerate(p.read_text(errors='replace').splitlines(),1):
   for ident in ids:
    if re.search(r'\b'+ident+r'\b|BRN_'+ident+r'\b',line):out[ident][kind].append({'path':str(p.relative_to(root)),'line':n,'text':line.strip()[:200]})
result={'status':'static source index; absence does not establish workaround unnecessary','references':out,'context_instructions':{'dma_ld_shregs_pco':'INDEX register MOV + indexed SHARED destination LD_REGBL, DRC0','dma_st_shregs_pco':'INDEX register MOV + indexed SHARED source ST32_REGBL, DRC0, idf from intrinsic flags','load_shared_reg_alloc_size_pco':'MOV special register SH_ALLOC_SIZE','source':'pco/pco_trans_nir.c'},'contract':{'reserved_bytes':48,'pointer_inputs':'size-address64 then store-address64 through vtxin intrinsics','allocation_size_unit':'16 dwords in common.cl','warning':'Instruction mapping and source presence do not prove ISA/workaround compatibility for22.49.21.16'}}
Path('/mnt/d/y5_linux_work/gpu-context-instruction-audit.json').write_text(json.dumps(result,indent=2)+'\n')
for ident in ids:
 print(ident, {k:len(v) for k,v in out[ident].items()})
 for kind in roots:
  for ref in out[ident][kind][:5]:print(kind,ref)
