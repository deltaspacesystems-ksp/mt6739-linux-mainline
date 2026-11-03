from pathlib import Path
import re
import sys
from collections import defaultdict
base = Path('D:/y5_linux_work')
errors = defaultdict(list)
for line in (base / ('out/' + (sys.argv[1] if len(sys.argv) > 1 else 'ddk19-all.log'))).read_text().splitlines():
    if 'error:' not in line:
        continue
    path = line.split(':', 1)[0]
    message = line.split('error:', 1)[1].strip()
    if message not in errors[path]:
        errors[path].append(message)
(base / 'out/ddk19-build-errors.json').write_text(__import__('json').dumps(errors, indent=2))
print('Error files:', len(errors))
for path, messages in errors.items():
    print(path + ': ' + ' | '.join(messages[:3]))
print('Built objects:', len(list((base / 'rogue-ddk19-port').rglob('*.o'))))
