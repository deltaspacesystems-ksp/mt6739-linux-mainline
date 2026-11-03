from pathlib import Path
from datetime import datetime,timezone
import time,json
from mfw_night_monitor import sample
rows=[]
p=Path(r'D:\y5_linux_work\out\charge-reconnect.json')
for i in range(4):
 r=sample();r['utc']=datetime.now(timezone.utc).isoformat();rows.append(r)
 p.write_text(json.dumps(rows,indent=2)+'\n',encoding='utf-8')
 print('session_s',r['service']['elapsed_s'],'kicks',r['service']['kicks'],'errors',r['service']['errors'],'status',r['status'],flush=True)
 if i<3:time.sleep(15)
assert rows[0]['service']['elapsed_s']<600
assert all(r['registers']['online']==1 and r['service']['blocks']==0 and r['service']['profile_ok']==1 and r['registers']['control']==0x78 and r['status']=='Charging' and r['registers']['fault']==0 for r in rows)
assert rows[-1]['service']['kicks']>rows[0]['service']['kicks']
assert rows[-1]['service']['errors']==rows[0]['service']['errors']
assert rows[-1]['uptime_s']>43200
print('PASS: physical USB reconnect reset session and resumed charging without reboot',flush=True)
