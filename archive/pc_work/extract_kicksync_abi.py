from pathlib import Path
import re
r=Path('/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19');s=(r/'generated/rgxkicksync_bridge/common_rgxkicksync_bridge.h').read_text();prefix=s[:s.index('#')]
names=['PVRSRV_BRIDGE_IN_RGXCREATEKICKSYNCCONTEXT','PVRSRV_BRIDGE_OUT_RGXCREATEKICKSYNCCONTEXT','PVRSRV_BRIDGE_IN_RGXDESTROYKICKSYNCCONTEXT','PVRSRV_BRIDGE_OUT_RGXDESTROYKICKSYNCCONTEXT','PVRSRV_BRIDGE_IN_RGXKICKSYNC','PVRSRV_BRIDGE_OUT_RGXKICKSYNC']
t=prefix+'\n/* Exact generated1.9 structs, dependency-free aliases for native standalone diagnostic. */\n#include <stdint.h>\ntypedef void* IMG_HANDLE;typedef uint32_t IMG_UINT32;typedef char IMG_CHAR;typedef int32_t PVRSRV_FENCE;typedef int32_t PVRSRV_TIMELINE;typedef uint32_t PVRSRV_ERROR;\n'
for n in names:t+=re.search(r'typedef struct '+n+r'_TAG\s*\{.*?\}\s*__attribute__\(\(packed\)\) '+n+r';',s,re.S).group()+'\n'
t+='''_Static_assert(sizeof(PVRSRV_BRIDGE_IN_RGXKICKSYNC)==108,"1.9 kick-sync wire");
_Static_assert(sizeof(PVRSRV_BRIDGE_OUT_RGXKICKSYNC)==8,"1.9 kick-sync return");
'''
Path('/mnt/d/y5_linux_work/mfw_kicksync_abi.h').write_text(t)

