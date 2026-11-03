from pathlib import Path
from phone_shell import run
import time
cmd='echo SAMPLE; cat /proc/uptime; cat /sys/bus/platform/devices/1000d000.pwrap/charger_service; cat /sys/bus/platform/devices/1000d000.pwrap/charger_registers; cat /sys/class/power_supply/microfw-battery/temp; cat /sys/class/power_supply/microfw-battery/voltage_now; cat /sys/class/power_supply/microfw-battery/status'
p=Path(r'D:\y5_linux_work\out\v53-first-boot.txt')
for i in range(12):
    out,rc,ok=run(cmd,15)
    lines=[x.strip() for x in out.splitlines() if x.strip() and not x.startswith(('exec ','~ #','> ')) and ('kicks=' in x or x.startswith('id=') or x.strip().replace('.','').replace(' ','').isdigit() or x.strip() in ('Charging','Full','Not charging','Discharging'))]
    text='\n'.join(lines)
    with p.open('a',encoding='utf-8') as f:f.write(text+'\n')
    print(text,flush=True)
    if not ok or str(rc)!='0': raise SystemExit('Phone shell failed')
    if i!=11:time.sleep(15)
