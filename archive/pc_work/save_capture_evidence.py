"""Save only explicitly selected, non-private hardware evidence."""
from pathlib import Path
import json, sys, shlex
w=Path(r'D:\y5_linux_work')
sys.path.insert(0,str(w))
from phone_shell import run
nodes={
 'i2c@11008000/alsps@53':['compatible','reg','status'],
 'i2c@11008000/cust_alsps@0':['compatible','i2c_num','polling_mode_als','polling_mode_ps','ps_threshold_high','ps_threshold_low'],
 'i2c@11009000/camera_main@43':['compatible','reg'],
 'i2c@11009000/camera_sub@41':['compatible','reg'],
 'audio@11220000':['compatible','reg'],
 'audio_sram@11221000':['compatible','reg'],
 'seninf@15008000':['compatible','reg'],
 'kd_camera_hw1@15008000':['compatible','reg','clock-names','pinctrl-names'],
 'gps':['compatible'],
}
commands=[]
for node,props in nodes.items():
 for prop in props:
  path=shlex.quote('/sys/firmware/devicetree/base/'+node+'/'+prop)
  commands.append(f"if [ -f {path} ]; then printf 'MFW_DT {node} {prop} '; od -An -v -tx1 {path} | tr -d '\\n'; echo; fi")
# Upload/run avoids interactive echo contaminating values split over long lines.
script=w/'out/read_capture_evidence.sh'
script.write_text('#!/bin/sh\n'+'\n'.join(commands)+'\n',newline='\n')
import subprocess
subprocess.run([sys.executable,str(w/'phone_push.py'),str(script),'/var/cache/mfw-ui/read_capture_evidence.sh','--port','9500','--mode','700'],check=True)
out,rc,ok=run('sh /var/cache/mfw-ui/read_capture_evidence.sh',30)
assert ok and str(rc)=='0'
result={}
for line in out.splitlines():
 if not line.startswith('MFW_DT '):continue
 _,node,prop,hexbytes=line.split(' ',3)
 assert node in nodes and prop in nodes[node]
 data=bytes.fromhex(hexbytes)
 if prop in ('compatible','clock-names','pinctrl-names','status'):
  value=[s.decode('ascii') for s in data.rstrip(b'\0').split(b'\0')]
 else:
  assert len(data)%4==0
  value=[int.from_bytes(data[i:i+4],'big') for i in range(0,len(data),4)]
 result.setdefault(node,{})[prop]=value
assert len(result)==len(nodes)
(w/'out/capture-live-dt.json').write_text(json.dumps(dict(
 source='Live DT, running v53; selected hardware properties only; no chosen/calibration data',
 nodes=result),indent=2)+'\n',encoding='utf-8')
print('PASS:',len(result),'named hardware nodes saved; no full DT exported')
