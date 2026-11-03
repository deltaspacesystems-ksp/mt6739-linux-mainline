from pathlib import Path
import json
b=Path('D:/y5_linux_work');m=json.loads((b/'out/graphics64_trial1/manifest.json').read_text());lines=['#!/bin/sh','set -eu','cd /tmp/mfw-graphics64',"cat <<'SUMS' | sha256sum -c -"]
for name,row in m['files'].items():lines.append(row['sha256']+'  '+name)
lines += ['SUMS','cc -std=c11 -O2 -Wall -Wextra -Werror /tmp/graphics19_stage_trial.c -o /tmp/graphics19-stage-trial','/tmp/graphics19-stage-trial','cat /sys/kernel/debug/pvr/status']
(b/'run_graphics64_stage.sh').write_text('\n'.join(lines)+'\n',newline='\n')
