"""Static numeric bridge-map audit; never equate equal IDs with equal ABI."""
from pathlib import Path
import ast
import json
import re
base=Path('D:/y5_linux_work')
reference=base/'sources/rogue-ddk19'
definitions={}
for p in [reference/'services/include/pvr_bridge.h',*sorted((reference/'generated').glob('*/common_*_bridge.h'))]:
    for name,value in re.findall(r'^#define\s+(PVRSRV_BRIDGE_[A-Z0-9_]+)\s+([^\n]+)',p.read_text(),re.M):
        definitions[name]=value.split('/*',1)[0].strip()
def value(expression,seen=()):
    expression=re.sub(r'(?<=\d)[uUlL]+\b','',expression)
    node=ast.parse(expression,mode='eval').body
    def walk(n):
        if isinstance(n,ast.Constant) and type(n.value) is int:return n.value
        if isinstance(n,ast.Name) and n.id in definitions and n.id not in seen:
            return value(definitions[n.id],(*seen,n.id))
        if isinstance(n,ast.BinOp) and isinstance(n.op,(ast.Add,ast.Sub)):
            left,right=walk(n.left),walk(n.right)
            return left+right if isinstance(n.op,ast.Add) else left-right
        raise ValueError(ast.dump(n))
    return walk(node)
entries=[]
mesa=(base/'sources/mesa-srv-reference/pvr_srv_bridge.h').read_text()
for name,expr in re.findall(r'^#define\s+(PVR_SRV_BRIDGE_[A-Z0-9_]+)\s+(\d+[uUlL]*)\s*$',mesa,re.M):
    target=name.replace('PVR_SRV_BRIDGE_','PVRSRV_BRIDGE_',1)
    expected=value(expr)
    actual=value(definitions[target]) if target in definitions else None
    entries.append({'mesa_117':name,'mesa_id':expected,'ddk_19_name':target,'ddk_19_id':actual,'numeric_id_same':actual==expected,'abi_reuse_allowed':False})
assert next(e for e in entries if e['mesa_117']=='PVR_SRV_BRIDGE_MM_HEAPCFGHEAPCOUNT')['ddk_19_id']==28
assert next(e for e in entries if e['mesa_117']=='PVR_SRV_BRIDGE_MM_HEAPCFGHEAPDETAILS')['ddk_19_id']==30
result={'mesa_commit':'2736dd1220d58192f5591ca6f9fcc5e60ee368ad','ddk_commit':'096323c2d5db8a05e84df31608fc0c9d22e47b31','scope':'static IDs only; check CONNECT capability masks for runtime availability; every wire structure requires independent adaptation','commands':entries,'additional_protocol_differences':['DDK1.9 initializes firmware on render OPEN; do not send Mesa1.17 DRM INIT ioctl','CONNECT1.9 returns capability/bridge masks, not BVNC','Native sync timeline control is on separate pvr_sync misc device in1.9','Heap details1.9 lack the1.17 reserved carveout-size field']}
(Path('X:/y5linux/phone/hardware/mesa-ddk19-bridge-command-map.json')).write_text(json.dumps(result,indent=2)+'\n',newline='\n')
print('Mapped',len(entries),'constants;',sum(e['ddk_19_id'] is None for e in entries),'missing names;',sum(e['ddk_19_id'] is not None and not e['numeric_id_same'] for e in entries),'changed numeric IDs; no ABI assumed')
