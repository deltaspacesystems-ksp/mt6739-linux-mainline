from pathlib import Path,PurePosixPath
import re,tarfile,difflib
w=Path('/mnt/d/y5_linux_work');k=Path('/home/y5/linux-7.3')
text=(w/'export/mfw-kernel-7.3-v57.patch').read_text()
headers=re.findall(r'^--- (a/[^\n]+|/dev/null)\n\+\+\+ b/([^\n]+)',text,re.M)
assert len(headers)==23
headers.append(('/dev/null','drivers/video/mfw-frame-switch.h'))
oldnames={name[2:] for name,_ in headers if name!='/dev/null'}
assert all('..' not in PurePosixPath(p).parts and not p.startswith('/') for p in oldnames)
originals={}
with tarfile.open('/home/y5/linux-7.3.tar.xz','r|xz') as archive:
 for member in archive:
  name=member.name.partition('/')[2]
  if name not in oldnames:continue
  assert member.isfile()
  originals[name]=archive.extractfile(member).read().decode()
  if len(originals)==len(oldnames):break
assert originals.keys()==oldnames
out=[]
for old,name in headers:
 before='' if old=='/dev/null' else originals[name]
 after=(k/name).read_text()
 part=''.join(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile=old,tofile='b/'+name))
 assert part,name
 out.append(part)
(w/'export/mfw-kernel-7.3-v58.patch').write_text(''.join(out),newline='\n')
print('Exported complete24-file patch from pristine7.3 originals')
