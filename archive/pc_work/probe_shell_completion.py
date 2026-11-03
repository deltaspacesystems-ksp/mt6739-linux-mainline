from pathlib import Path
import json
from phone_shell import run
from mfw_night_monitor import CMD
for i in range(20):
 out,rc,ok=run(CMD,20)
 print('attempt',i,'rc',repr(rc),'complete',ok,flush=True)
 if not ok or str(rc)!='0':
  p=Path(r'D:\y5_linux_work\out\shell-marker-failure.json')
  p.write_text(json.dumps(dict(attempt=i,rc=rc,complete=ok,tail=out[-500:]),indent=2)+'\n',encoding='utf-8')
  print('Captured diagnostic tail to local file',flush=True)
  break
