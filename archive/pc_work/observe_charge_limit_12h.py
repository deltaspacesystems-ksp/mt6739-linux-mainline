from pathlib import Path
from datetime import datetime,timezone
import json,time
from mfw_night_monitor import sample
rows=[]
p=Path(r'D:\y5_linux_work\out\charge-limit-12h.json')
for i in range(7):
 row=sample();row['utc']=datetime.now(timezone.utc).isoformat();rows.append(row)
 p.write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
 print('elapsed',row['service']['elapsed_s'],'blocks',row['service']['blocks'],'control',hex(row['registers']['control']),'status',row['status'],'kicks',row['service']['kicks'],flush=True)
 if i<6:time.sleep(15)
after=[r for r in rows if r['service']['elapsed_s']>=43200]
assert len(after)>=2,'Threshold not observed long enough'
assert all(r['service']['blocks']&2 and r['registers']['control']&4 and r['status']=='Not charging' for r in after)
assert after[-1]['service']['kicks']>after[0]['service']['kicks']
assert after[-1]['service']['errors']==rows[0]['service']['errors']
assert all(r['registers']['fault']==0 for r in after)
print('PASS: real 12h limit disabled CE, watchdog continued, no new service errors',flush=True)
