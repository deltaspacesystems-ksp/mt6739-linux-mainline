from pathlib import Path
import json,hashlib,struct
r=Path('X:/y5linux');d=json.loads((r/'phone/hardware/gpu-exact-firmware-compute-dwarf.json').read_text());names={'RGXFWIF_CDM_REGISTERS':'mfw_cdm19_fw_regs','RGXFWIF_CDM_REGISTERS_CSWITCH':'mfw_cdm19_fw_ctx','RGXFWIF_CMD_COMPUTE':'mfw_cdm19_fw_cmd'}
lines=['/* SPDX-License-Identifier: MIT */','/* Wire layout independently recovered from exact firmware DWARF. No device operations. */','#ifndef MFW_CDM19_FW_WIRE_H','#define MFW_CDM19_FW_WIRE_H','#include <stdint.h>','#include <stddef.h>']
for key,newname in names.items():
 t=d['types'][key];lines.append('struct '+newname+' {')
 for m in t['members']:
  typ='uint64_t' if m['bytes']==8 else 'uint32_t' if m['bytes']==4 else 'struct '+names['RGXFWIF_CDM_REGISTERS' if m['name']=='sCDMRegs' else 'RGXFWIF_CDM_REGISTERS_CSWITCH']
  lines.append(' '+typ+' '+m['name']+';')
 lines.append('};');lines.append('_Static_assert(sizeof(struct '+newname+')=='+str(t['bytes'])+',"exact firmware size");')
 for m in t['members']:lines.append('_Static_assert(offsetof(struct '+newname+','+m['name']+')=='+str(m['offset'])+',"exact firmware member offset");')
lines+=['#endif',''];Path('D:/y5_linux_work/mfw_cdm19_fw_wire.h').write_text('\n'.join(lines))
# Gather only native compiler outputs, never vendor binaries.
base=Path('D:/y5_linux_work/out');main=Path('//wsl.localhost/archlinux/tmp/mfw-pco-restricted.bin').read_bytes();assert hashlib.sha256(main).hexdigest()=='3c15b86129d126c263fbd831312216f468ba4897068c95d7b8ea23fdeddba646'
ctx=json.loads((base/'context20-result.json').read_text());image=bytearray(4096)
for off,data in [(0,main),(0x200,(base/'cs_store_sr_1024_common-bounded.bin').read_bytes()),(0x600,(base/'cs_load_sr_256_common-bounded.bin').read_bytes())]:
 assert off+len(data)<=4096;image[off:off+len(data)]=data
(base/'compute-usc-candidate.bin').write_bytes(image)
files=['compute-pds-candidate.bin','compute-usc-candidate.bin','compute-stream-candidate.bin'];manifest={n:{'bytes':(base/n).stat().st_size,'sha256':hashlib.sha256((base/n).read_bytes()).hexdigest()} for n in files}
(base/'compute-bundle-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(manifest,indent=2))
