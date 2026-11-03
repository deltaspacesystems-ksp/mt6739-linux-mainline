"""Dry-run complete v60 patch on selected pristine 7.3 originals."""
from pathlib import Path, PurePosixPath
import re
import subprocess
import tarfile
import tempfile
patch = Path('/mnt/d/y5_linux_work/export/mfw-kernel-7.3-v60.patch')
headers = re.findall(r'^--- (a/[^\n]+|/dev/null)\n\+\+\+ b/([^\n]+)', patch.read_text(), re.M)
assert len(headers) == 627
oldnames = {old[2:] for old,_ in headers if old != '/dev/null'}
assert all(not name.startswith('/') and '..' not in PurePosixPath(name).parts for name in oldnames)
with tempfile.TemporaryDirectory(prefix='mfw-v60-check-') as directory:
    root = Path(directory)
    pending = set(oldnames)
    with tarfile.open('/home/y5/linux-7.3.tar.xz', 'r|xz') as archive:
        for member in archive:
            name = member.name.partition('/')[2]
            if name not in pending: continue
            assert member.isfile()
            target = root / name
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.extractfile(member).read())
            pending.remove(name)
            if not pending: break
    assert not pending
    result = subprocess.run(['patch', '--dry-run', '--batch', '-p1', '-i', str(patch)], cwd=root, text=True, capture_output=True)
    if result.returncode:
        print(result.stdout[-4000:], result.stderr)
        raise SystemExit(result.returncode)
print('PASS: complete627-file v60 patch applies to pristine7.3 originals')
