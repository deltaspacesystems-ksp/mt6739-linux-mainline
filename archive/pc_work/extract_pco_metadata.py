from pathlib import Path
import re
r=Path('/home/y5/mesa-native');s=(r/'src/imagination/vulkan/pvr_arch_pipeline.c').read_text()
names=['allocate_var','try_allocate_var','allocate_val','try_allocate_vars','check_unused_sysvals','pvr_alloc_vs_sysvals','pvr_alloc_vs_attribs','pvr_alloc_vs_varyings','pvr_alloc_fs_sysvals','pvr_alloc_fs_varyings','pvr_alloc_cs_sysvals']
out=s[:s.index('#include')]+ '\n/* Offline probe: exact metadata allocators from pinned Mesa; no Vulkan/device initialization. */\n#include "util/bitscan.h"\n#include "hwdef/rogue_hw_defs.h"\n'
for name in names:
 m=re.search(r'\b'+name+r'\([^;]*?\)\s*\{',s,re.S);assert m,name
 start=s.rfind('\nstatic ',0,m.start())+1;cur=s.index('{',m.start());depth=1;end=cur+1
 while depth:
  if s[end]=='{':depth+=1
  elif s[end]=='}':depth-=1
  end+=1
 out+=s[start:end]+'\n'
Path('/mnt/d/y5_linux_work/pco_metadata_allocators.h').write_text(out)
p=Path('/mnt/d/y5_linux_work/pco_restricted_probe.c');t=p.read_text().replace('#include <stdlib.h>','#include <stdlib.h>\n#include "pco_metadata_allocators.h"')
t=t.replace('data.common.sys_vals[SYSTEM_VALUE_VERTEX_ID]=(pco_range){.start=0,.count=1};data.common.vtxins=1;','').replace('data.vs.varyings[VARYING_SLOT_POS]=(pco_range){.start=0,.count=4};data.vs.vtxouts=4;','')
a=' pco_shader *shader=pco_trans_nir';inject=''' if(stage==1){pvr_alloc_vs_sysvals(&data,b.shader);pvr_alloc_vs_attribs(&data,b.shader);pvr_alloc_vs_varyings(&data,b.shader);}
 else if(stage==2){pvr_alloc_fs_sysvals(&data,b.shader);pvr_alloc_fs_varyings(&data,b.shader);}
 else pvr_alloc_cs_sysvals(&data,b.shader);
''';assert a in t;t=t.replace(a,inject+a);p.write_text(t)
