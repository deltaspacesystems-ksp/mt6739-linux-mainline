from pathlib import Path
import subprocess,hashlib,json
rows=[]
for stage in [0,1,2]:
 for depth in [16,32,64,128,208,256]:
  for slots in [4,8,16,32,64]:
   p=subprocess.run(['/tmp/mfw-pco-restricted',str(depth),str(slots),str(stage)],capture_output=True,text=True)
   data=Path('/tmp/mfw-pco-restricted.bin').read_bytes() if p.returncode==0 else b''
   rows.append({'stage':['compute','vertex','fragment'][stage],'synthetic_depth':depth,'synthetic_slots':slots,'rc':p.returncode,'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest() if data else None})
result={'scope':'offline synthetic sensitivity sweep, normal shaders internal=false; NOT hardware profile or validation','target_ident_only':'22.49.21.16','cases':rows,'stage_summary':{},'hardware_executed':False,'warning':'unknown features/errata unresolved; sampled-limit invariance does not establish target compatibility; no upload or dispatch'}
for stage in ['compute','vertex','fragment']:
 rs=[x for x in rows if x['stage']==stage];result['stage_summary'][stage]={'successes':sum(x['rc']==0 for x in rs),'cases':len(rs),'unique_success_binaries':len(set(x['sha256'] for x in rs if x['rc']==0)),'sizes':sorted(set(x['bytes'] for x in rs if x['rc']==0))}
Path('/mnt/d/y5_linux_work/out/pco-normal-shader-sensitivity.json').write_text(json.dumps(result,indent=2)+'\n');print(result['stage_summary'])
