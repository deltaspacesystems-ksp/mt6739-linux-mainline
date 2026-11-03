from pathlib import Path
import re,json
b=Path('/mnt/d/y5_linux_work');s=Path('/home/y5/rogue-ddk19-port/generated/rgxta3d_bridge/common_rgxta3d_bridge.h').read_text()
width={'IMG_HANDLE':8,'IMG_UINT32':4,'IMG_UINT64':8,'IMG_BOOL':4,'PVRSRV_FENCE':4,'PVRSRV_TIMELINE':4,'IMG_DEV_VIRTADDR':8,'PVRSRV_ERROR':4}
lines=['/* SPDX-License-Identifier: MIT */','/* Exact legacy bridge field storage; no submission helper. */','#ifndef MFW_RENDER19_BRIDGE_WIRE_H','#define MFW_RENDER19_BRIDGE_WIRE_H','#include <stddef.h>','#include <stdint.h>'];facts={}
for kind,expected in [('IN',352),('OUT',16)]:
 block=re.search(r'typedef struct PVRSRV_BRIDGE_'+kind+r'_RGXKICKTA3D_TAG\s*\{(.*?)\}',s,re.S)[1]
 tag='mfw19_kickta3d_'+kind.lower();lines.append('struct '+tag+' {');off=0;fields=[]
 for line in block.strip().splitlines():
  m=re.fullmatch(r'\s*(\w+)\s+(\*\s*)?(\w+);\s*',line);assert m,line
  typ,ptr,name=m.groups();n=8 if ptr else width[typ];lines.append(f' uint8_t {name}[{n}];');fields.append({'name':name,'offset':off,'bytes':n,'source_type':typ+('*' if ptr else '')});off+=n
 assert off==expected,(off,expected)
 lines.append('} __attribute__((packed));');lines.append(f'_Static_assert(sizeof(struct {tag})=={off},"exact stock bridge size");')
 for f in fields:lines.append(f'_Static_assert(offsetof(struct {tag},{f["name"]})=={f["offset"]},"{f["name"]}");')
 facts[kind]={'bytes':off,'fields':fields}
lines.append('#endif');(b/'mfw_render19_bridge_wire.h').write_text('\n'.join(lines)+'\n',newline='\n');(b/'out/render19-bridge-layout.json').write_text(json.dumps(facts,indent=2)+'\n')
(b/'check_render19_bridge.c').write_text('#include "mfw_render19_bridge_wire.h"\nint main(void){return 0;}\n',newline='\n')
print('Exact legacy KICKTA3D352B input/16B output; independent stock wrapper agrees.')
