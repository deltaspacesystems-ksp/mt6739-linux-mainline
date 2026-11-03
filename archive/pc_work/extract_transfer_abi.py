from pathlib import Path
import re
r=Path('/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19');s=(r/'generated/rgxtq_bridge/common_rgxtq_bridge.h').read_text()
t='#include <stddef.h>\n#include "img_types.h"\n#include "pvrsrv_error.h"\n#include "rgx_fwif_resetframework.h"\n'
for n in ['PVRSRV_BRIDGE_IN_RGXCREATETRANSFERCONTEXT','PVRSRV_BRIDGE_OUT_RGXCREATETRANSFERCONTEXT']:
 t+=re.search(r'typedef struct '+n+r'_TAG\s*\{.*?\}\s*__attribute__\(\(packed\)\) '+n+r';',s,re.S).group()+'\n'
t+='''_Static_assert(sizeof(PVRSRV_BRIDGE_IN_RGXCREATETRANSFERCONTEXT)==24,"create ABI");
_Static_assert(sizeof(PVRSRV_BRIDGE_OUT_RGXCREATETRANSFERCONTEXT)==12,"return ABI");
_Static_assert(sizeof(RGXFWIF_RF_CMD)==16,"disabled reset framework size");
_Static_assert(offsetof(RGXFWIF_RF_CMD,ui32Flags)==0,"disabled flags offset");
_Static_assert(RGXFWIF_RF_FLAG_ENABLE==1,"enable flag");
'''
Path('/mnt/d/y5_linux_work/transfer_abi_assert.c').write_text(t)
