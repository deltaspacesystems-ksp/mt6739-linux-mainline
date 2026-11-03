from pathlib import Path
import re,json
r=Path('/home/y5/mesa-native');h=(r/'src/imagination/common/pvr_device_info.h').read_text();start=h.index('struct pvr_device_features {');end=h.index('\n};',start);body=h[start:end]
fields=re.findall(r'\b(?:bool|uint\d+_t)\s+(\w+)\s*(?::\s*1)?\s*;',body)
known=json.loads(Path('/mnt/x/y5linux/phone/hardware/gpu-ge8100-feature-reference.json').read_text()) if Path('/mnt/x/y5linux').exists() else json.loads(Path('/mnt/d/y5_linux_work/out/ge8100-profile-input.json').read_text())
evidence=known['mesa_fields_with_direct_name_and_value_evidence']
uses={}
for folder in ['src/imagination/pco','src/imagination/vulkan','src/imagination/common','src/imagination/include']:
 for p in (r/folder).rglob('*'):
  if p.suffix not in ('.c','.h'):continue
  if '/device_info/' in str(p):continue
  source=p.read_text(errors='replace')
  for match in re.finditer(r'\b(PVR_HAS_FEATURE|PVR_GET_FEATURE_VALUE|PVR_FEATURE_VALUE|PVR_GET_FEATURE_VALUE_DEFAULT)\s*\([^,]+,\s*(\w+)',source):
   macro,name=match.groups();key=('has_'+name) if macro=='PVR_HAS_FEATURE' else name
   uses.setdefault(key,[]).append({'path':str(p.relative_to(r)),'line':source.count('\n',0,match.start())+1,'macro':macro})
rows=[]
for f in fields:
 rows.append({'field':f,'status':'reference-confirmed' if f in evidence else 'UNKNOWN','value':evidence.get(f),'uses':uses.get(f,[])})
out={'target':'22.49.21.16','scope':'direct evidence only; not a runnable device profile; unknown is not false','mesa_commit':'2736dd1220d58192f5591ca6f9fcc5e60ee368ad','fields':rows,'counts':{'declared':len(rows),'known':sum(x['status']!='UNKNOWN' for x in rows),'unknown':sum(x['status']=='UNKNOWN' for x in rows)},'parser_limit':'direct macro calls including multiline; not complete transitive reachability'}
Path('/mnt/d/y5_linux_work/out/gpu-profile-gap-audit.json').write_text(json.dumps(out,indent=2)+'\n')
print(out['counts'])
for x in rows:
 if x['status']=='UNKNOWN' and any('/pco/' in u['path'] for u in x['uses']):print('COMPILER UNKNOWN',x['field'],len(x['uses']))
