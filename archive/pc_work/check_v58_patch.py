"""Run under WSL: dry-run the complete patch against pristine archive files."""
from pathlib import Path,PurePosixPath
import re,tarfile,tempfile,subprocess
patch=Path('/mnt/d/y5_linux_work/export/mfw-kernel-7.3-v58.patch')
text=patch.read_text()
headers=re.findall(r'^--- (a/[^\n]+|/dev/null)\n\+\+\+ b/([^\n]+)',text,re.M)
assert len(headers)==24,len(headers)
old={name[2:] for name,_ in headers if name!='/dev/null'}
assert all('..' not in PurePosixPath(p).parts and not p.startswith('/') for p in old)
with tempfile.TemporaryDirectory(prefix='mfw-v58-check-') as directory:
 target=Path(directory);pending=set(old)
 with tarfile.open('/home/y5/linux-7.3.tar.xz','r|xz') as archive:
  for member in archive:
   name=member.name.partition('/')[2]
   if name not in pending:continue
   assert member.isfile()
   p=target/name;p.parent.mkdir(parents=True,exist_ok=True)
   p.write_bytes(archive.extractfile(member).read());pending.remove(name)
   if not pending:break
 assert not pending,pending
 result=subprocess.run(['patch','--dry-run','-p1','--input',str(patch)],cwd=target,text=True,capture_output=True)
 if result.returncode:print(result.stdout,result.stderr);raise SystemExit(result.returncode)
 print('PASS: complete24-file v58 patch applies to pristine7.3 archive sources')
