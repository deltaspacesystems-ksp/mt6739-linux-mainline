from pathlib import Path
import json,re
b=Path('D:/y5_linux_work');r=Path('X:/y5linux');j=json.loads((r/'phone/hardware/gpu-exact-firmware-render-dwarf.json').read_text())
lines=['/* SPDX-License-Identifier: MIT */','/* Exact loaded firmware DWARF storage layout. No command submission. */','#ifndef MFW_RENDER19_FW_WIRE_H','#define MFW_RENDER19_FW_WIRE_H','#include <stdint.h>','#include <stddef.h>']
for name in ['RGXFWIF_TAREGISTERS','RGXFWIF_3DREGISTERS','RGXFWIF_CMDTA','RGXFWIF_CMD3D']:
 t=j['types'][name];tag='mfw19_'+name.lower();lines.append('struct '+tag+' {');pos=0;fields=[]
 for m in t['members']:
  off=m['offset'];size=m['bytes']
  if size is None: continue # Preserve unknown array as opaque gap, never infer semantic length.
  if off>pos:lines.append(f' uint8_t opaque_{pos}[{off-pos}];')
  assert off>=pos
  n=m['name'];lines.append(f' uint8_t {n}[{size}];');fields.append((n,off));pos=off+size
 if pos<t['bytes']:lines.append(f' uint8_t opaque_{pos}[{t["bytes"]-pos}];')
 lines.append('} __attribute__((packed));');lines.append(f'_Static_assert(sizeof(struct {tag})=={t["bytes"]},"{name} size");')
 for n,off in fields:lines.append(f'_Static_assert(offsetof(struct {tag},{n})=={off},"{name}.{n}");')
lines.append('#endif');(b/'mfw_render19_fw_wire.h').write_text('\n'.join(lines)+'\n',newline='\n')
(b/'check_render19_wire.c').write_text('#include "mfw_render19_fw_wire.h"\nint main(void){return 0;}\n',newline='\n')
print('Generated exact storage layouts, unknown array retained opaque; no submission API.')
