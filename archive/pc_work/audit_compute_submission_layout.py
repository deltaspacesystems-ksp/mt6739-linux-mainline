from pathlib import Path
import re,subprocess,json
b=Path('/mnt/d/y5_linux_work');r=Path('/home/y5/linux-7.3/drivers/gpu/drm/mfw-rogue-ddk19');old=(r/'generated/rgxcmp_bridge/common_rgxcmp_bridge.h').read_text();modern=Path('/home/y5/mesa-native/src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_bridge.h').read_text()
t='#include <stdint.h>\n#include <stdio.h>\n#include <stddef.h>\ntypedef void* IMG_HANDLE;typedef uint32_t IMG_UINT32;typedef uint8_t IMG_BYTE;typedef char IMG_CHAR;typedef int32_t PVRSRV_FENCE,PVRSRV_TIMELINE;typedef uint32_t PVRSRV_ERROR;typedef struct {uint64_t uiAddr;} IMG_DEV_VIRTADDR;\n#define PACKED __attribute__((packed))\ntypedef struct {uint64_t addr;} pvr_dev_addr_t;\n'
for n in ['PVRSRV_BRIDGE_IN_RGXCREATECOMPUTECONTEXT','PVRSRV_BRIDGE_IN_RGXKICKCDM']:
 t+=re.search(r'typedef struct '+n+r'_TAG\s*\{.*?\}\s*__attribute__\(\(packed\)\) '+n+r';',old,re.S).group()+'\n'
for n in ['pvr_srv_rgx_create_compute_context_cmd','pvr_srv_rgx_kick_cdm2_cmd']:
 m=re.search(r'struct '+n+r' \{.*?\}\s*PACKED;',modern,re.S);assert m,n;t+=m.group()+'\n'
t+='''_Static_assert(sizeof(void*)==8,"64-bit ABI");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_RGXCREATECOMPUTECONTEXT)==32,"old create");
_Static_assert(sizeof(PVRSRV_BRIDGE_IN_RGXKICKCDM)==132,"old kick");
int main(void){printf("old_create=%zu old_kick=%zu modern_create=%zu modern_kick=%zu old_cmd_ptr_offset=%zu old_size_offset=%zu\\n",sizeof(PVRSRV_BRIDGE_IN_RGXCREATECOMPUTECONTEXT),sizeof(PVRSRV_BRIDGE_IN_RGXKICKCDM),sizeof(struct pvr_srv_rgx_create_compute_context_cmd),sizeof(struct pvr_srv_rgx_kick_cdm2_cmd),offsetof(PVRSRV_BRIDGE_IN_RGXKICKCDM,psDMCmd),offsetof(PVRSRV_BRIDGE_IN_RGXKICKCDM,ui32CmdSize));}
'''
(b/'compute_submission_layout.c').write_text(t);subprocess.run(['cc','-Wall','-Wextra','-Werror',str(b/'compute_submission_layout.c'),'-o','/tmp/mfw-compute-layout'],check=True);subprocess.run(['/tmp/mfw-compute-layout'],check=True)
