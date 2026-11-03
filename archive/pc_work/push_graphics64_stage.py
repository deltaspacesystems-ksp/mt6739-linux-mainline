from pathlib import Path
import subprocess,sys
b=Path('D:/y5_linux_work');p=b/'out/graphics64_trial1'
for f in sorted(p.glob('*.bin')):
 subprocess.run([sys.executable,str(b/'phone_push.py'),str(f),'/tmp/mfw-graphics64/'+f.name,'--port','9500','--mode','644'],check=True)
subprocess.run([sys.executable,str(b/'phone_push.py'),str(b/'graphics19_stage_trial.c'),'/tmp/graphics19_stage_trial.c','--port','9500','--mode','644'],check=True)
