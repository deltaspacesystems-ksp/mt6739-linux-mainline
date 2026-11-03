"""Summarize the bounded, identifier-free PC telemetry log. No device access."""
from pathlib import Path
import json
import statistics

work=Path(r'D:\y5_linux_work')
rows=[json.loads(x) for x in (work/'out/mfw-night-telemetry.jsonl').read_text(encoding='utf-8').splitlines() if x.strip()]
good=[x for x in rows if 'service' in x and 'registers' in x]
assert good,'No successful telemetry samples'
first,last=good[0],good[-1]
summary=dict(samples=len(rows),successful=len(good),failed=len(rows)-len(good),
             first_utc=first['utc'],last_utc=last['utc'],
             observed_uptime_hours=(last['uptime_s']-first['uptime_s'])/3600,
             uptime_decreases=sum(b['uptime_s']<a['uptime_s'] for a,b in zip(good,good[1:])),
             voltage_uv=dict(first=first['voltage_uv'],last=last['voltage_uv'],
                             minimum=min(x['voltage_uv'] for x in good),maximum=max(x['voltage_uv'] for x in good)),
             temperature_deci_c=dict(minimum=min(x['temp_deci_c'] for x in good),maximum=max(x['temp_deci_c'] for x in good)),
             status_values=sorted({x['status'] for x in good}),
             service_first=first['service'],service_last=last['service'],
             observed_fault_values=sorted({x['registers']['fault'] for x in good}),
             nonzero_service_errors=sum(x['service']['last_error']!=0 for x in good),
             blocked_samples=sum(x['service']['blocks']!=0 for x in good),
             unsupported_profile_samples=sum(x['service']['profile_ok']!=1 for x in good),
             observed_oreg_values=sorted({x['registers']['oreg'] for x in good}),
             largest_gap_s=max((b['uptime_s']-a['uptime_s'] for a,b in zip(good,good[1:])),default=0),
             interpretation='Observed samples only; not a current measurement or proof of a completed charge cycle. VBAT ADC calibration and actual charger CV need comparison.')
text=json.dumps(summary,indent=2)+'\n'
(work/'out/mfw-night-summary.json').write_text(text,encoding='utf-8')
print(text)
