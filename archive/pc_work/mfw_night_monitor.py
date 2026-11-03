"""Bounded, read-only overnight MICROFW telemetry on the PC. No device writes/resets."""
import argparse
import datetime as dt
import json
from pathlib import Path
import re
import os
import time
from phone_shell import run

CMD = '''printf 'MFW_UPTIME='; cut -d' ' -f1 /proc/uptime
printf 'MFW_VBAT='; cat /sys/class/power_supply/microfw-battery/voltage_now
printf 'MFW_TEMP='; cat /sys/class/power_supply/microfw-battery/temp
printf 'MFW_STATUS='; cat /sys/class/power_supply/microfw-battery/status
printf 'MFW_SERVICE='; cat /sys/bus/platform/devices/1000d000.pwrap/charger_service
printf 'MFW_REGS='; cat /sys/bus/platform/devices/1000d000.pwrap/charger_registers
printf 'MFW_CPU='; cat /sys/bus/platform/devices/mfw-cpu-clock/cpu_rate_hz
'''

def sample():
    out,rc,ok=run(CMD,20)
    if not ok or str(rc)!='0':
        raise RuntimeError('phone telemetry failed')
    def field(name, pattern):
        m=re.search(r'^MFW_'+name+'=('+pattern+r')\s*$',out,re.M)
        if not m: raise RuntimeError('missing/invalid '+name)
        return m.group(1).strip()
    service=field('SERVICE',r'(?:[a-z_]+=-?\d+ ?)+')
    regs=field('REGS',r'(?:[a-z_]+=(?:0x[0-9a-f]+|\d+) ?)+')
    return dict(uptime_s=float(field('UPTIME',r'\d+\.\d+')),
                voltage_uv=int(field('VBAT',r'\d+')),
                temp_deci_c=int(field('TEMP',r'-?\d+')),
                status=field('STATUS',r'Charging|Full|Not charging|Discharging|Unknown'),
                cpu_hz=int(field('CPU',r'\d+')),
                service={k:int(v) for k,v in re.findall(r'([a-z_]+)=(-?\d+)',service)},
                registers={k:int(v,16) if v.startswith('0x') else int(v) for k,v in re.findall(r'([a-z_]+)=(0x[0-9a-f]+|\d+)',regs)})

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--hours',type=float,default=10)
    ap.add_argument('--interval',type=float,default=60)
    ap.add_argument('--once',action='store_true')
    args=ap.parse_args()
    root=Path(r'D:\y5_linux_work\out')
    stop=root/'mfw-night-monitor.stop'
    log=root/'mfw-night-telemetry.jsonl'
    latest=root/'mfw-night-latest.json'
    if args.once:
        print(json.dumps(sample(),ensure_ascii=True)); return
    deadline=time.monotonic()+args.hours*3600
    (root/'mfw-night-monitor.pid').write_text(str(os.getpid()),encoding='ascii')
    while time.monotonic()<deadline and not stop.exists():
        row=dict(utc=dt.datetime.now(dt.timezone.utc).isoformat())
        try:row.update(sample())
        except Exception as exc:row['error']=type(exc).__name__+': '+str(exc)
        text=json.dumps(row,ensure_ascii=True)
        with log.open('a',encoding='utf-8') as f:f.write(text+'\n')
        temp=latest.with_suffix('.tmp')
        temp.write_text(text+'\n',encoding='utf-8');temp.replace(latest)
        remaining=args.interval
        while remaining>0 and not stop.exists() and time.monotonic()<deadline:
            delay=min(remaining,1);time.sleep(delay);remaining-=delay

if __name__=='__main__':main()
