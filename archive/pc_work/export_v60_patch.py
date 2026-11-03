"""Export complete self-contained kernel patch, excluding generated/binary files."""
from pathlib import Path, PurePosixPath
import difflib
import json
import re
import tarfile
base = Path('/mnt/d/y5_linux_work')
k = Path('/home/y5/linux-7.3')
previous = (base / 'export/mfw-kernel-7.3-v59.patch').read_text()
headers = re.findall(r'^--- (a/[^\n]+|/dev/null)\n\+\+\+ b/([^\n]+)', previous, re.M)
assert len(headers) == 31
headers.append(('a/drivers/gpu/drm/Kconfig', 'drivers/gpu/drm/Kconfig'))
manifest = json.loads((base / 'out/ddk19-source-tree.json').read_text())
paths = [entry['path'] for entry in manifest['tree']['tree'] if entry['type'] == 'blob']
paths += ['services/server/env/linux/mfw-dmabuf-fill.h', 'include/mfw-linux-compat.h', 'include/mfw-ddk-power.h', 'include/mfw-mfg-power-sequence.h', 'services/system/rgx_mtk/mfw-ddk-power.c']
headers += [('/dev/null', 'drivers/gpu/drm/mfw-rogue-ddk19/' + relative) for relative in paths]
assert len(headers) == 627 and len(set(name for _,name in headers)) == 627
oldnames = {old[2:] for old,_ in headers if old != '/dev/null'}
for _,name in headers:
    assert not name.startswith('/') and '..' not in PurePosixPath(name).parts
originals = {}
with tarfile.open('/home/y5/linux-7.3.tar.xz', 'r|xz') as archive:
    for member in archive:
        name = member.name.partition('/')[2]
        if name not in oldnames: continue
        assert member.isfile()
        originals[name] = archive.extractfile(member).read().decode()
        if len(originals) == len(oldnames): break
assert set(originals) == oldnames
output = []
for old,name in headers:
    before = '' if old == '/dev/null' else originals[name]
    after = (k / name).read_bytes().decode()
    lines = list(difflib.unified_diff(before.splitlines(True), after.splitlines(True), fromfile=old, tofile='b/' + name))
    assert lines, name
    for line in lines:
        output.append(line if line.endswith('\n') else line + '\n\\ No newline at end of file\n')
patch = base / 'export/mfw-kernel-7.3-v60.patch'
patch.write_text(''.join(output), newline='\n')
print('Exported complete627-file v60 patch:', patch.stat().st_size, 'bytes, source only')
