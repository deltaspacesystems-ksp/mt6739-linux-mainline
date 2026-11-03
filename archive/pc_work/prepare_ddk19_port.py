"""Verify pinned public sources and create an isolated Linux 7.3 port tree."""
import hashlib
import json
import shutil
from pathlib import Path

BASE = Path('D:/y5_linux_work')
SOURCE = BASE / 'sources/rogue-ddk19'
PORT = BASE / 'rogue-ddk19-port'
manifest = json.loads((BASE / 'out/ddk19-source-tree.json').read_text())
tree = manifest['tree']
if isinstance(tree, dict):
    tree = tree['tree']
errors = []
count = 0
for entry in tree:
    if entry['type'] != 'blob':
        continue
    relative = Path(entry['path'])
    if relative.is_absolute() or '..' in relative.parts:
        raise ValueError(relative)
    data = (SOURCE / relative).read_bytes()
    digest = hashlib.sha1(b'blob ' + str(len(data)).encode() + b'\0' + data).hexdigest()
    if digest != entry['sha']:
        # apply_patch adds a final newline; restore only if Git proves exact bytes.
        candidate = data[:-1] if data.endswith(b'\n') else data
        candidate_digest = hashlib.sha1(b'blob ' + str(len(candidate)).encode() + b'\0' + candidate).hexdigest()
        if candidate_digest == entry['sha']:
            (SOURCE / relative).write_bytes(candidate)
        else:
            errors.append(str(relative))
    count += 1
if errors:
    raise SystemExit('Git blob mismatch: ' + ', '.join(errors))
PORT.mkdir(exist_ok=True)
for entry in tree:
    if entry['type'] == 'blob':
        target = PORT / entry['path']
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(SOURCE / entry['path'], target)
print(f'PASS: {count} pinned Git blobs verified; isolated port tree prepared')
