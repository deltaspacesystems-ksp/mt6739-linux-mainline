from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import urllib.request,re,json,tarfile,hashlib
from phone_shell import run
root=Path(r'D:\y5_linux_work\alpine-ui-packages')
requested='labwc foot wayland-dev cairo-dev pango-dev wayland-protocols build-base pkgconf font-dejavu seatd'
cmd='apk --repositories-file /tmp/mfw-ui-repositories --no-network fetch --recursive --url --simulate '+requested
out,rc,ok=run(cmd,90)
assert ok and str(rc)=='0','Native apk solver failed'
selected=sorted(set(re.findall(r'/tmp/mfw-apk/(main|community)/aarch64/([A-Za-z0-9+_.~\-]+\.apk)',out)))
assert len(selected)>30
metadata={}
for repo in ('main','community'):
 with tarfile.open(root/repo/'APKINDEX.tar.gz','r:gz') as t:index=t.extractfile('APKINDEX').read().decode()
 for entry in index.split('\n\n'):
  f=dict(x.split(':',1) for x in entry.splitlines() if ':' in x)
  if 'P' in f and 'V' in f:metadata[(repo,f['P']+'-'+f['V']+'.apk')]=f
size=sum(int(metadata[x]['S']) for x in selected)
print('Native solver selected',len(selected),'packages,',round(size/1024/1024,1),'MiB download',flush=True)
def one(item):
 repo,name=item;p=root/repo/name;m=metadata[item]
 if not p.exists() or p.stat().st_size!=int(m['S']):
  url='https://dl-cdn.alpinelinux.org/alpine/v3.24/'+repo+'/aarch64/'+name
  blob=urllib.request.urlopen(url,timeout=60).read(int(m['S'])+1)
  assert len(blob)==int(m['S']),name
  p.write_bytes(blob)
 return dict(repository=repo,file=name,package=m['P'],version=m['V'],bytes=p.stat().st_size,sha256=hashlib.sha256(p.read_bytes()).hexdigest())
rows=[]
with ThreadPoolExecutor(max_workers=6) as pool:
 futures=[pool.submit(one,x) for x in selected]
 for f in as_completed(futures):
  rows.append(f.result())
  if len(rows)%20==0 or len(rows)==len(selected):print('Downloaded',len(rows),'/',len(selected),flush=True)
rows.sort(key=lambda x:(x['repository'],x['file']))
(root/'manifest.json').write_text(json.dumps(dict(arch='aarch64',branch='v3.24',requested=requested.split(),packages=rows),indent=2)+'\n',encoding='utf-8')
archive=root/'mfw-ui-packages.tar'
with tarfile.open(archive,'w') as t:
 for r in rows:t.add(root/r['repository']/r['file'],arcname=r['repository']+'/aarch64/'+r['file'],recursive=False)
print('Ready:',archive.stat().st_size,'bytes; SHA256',hashlib.sha256(archive.read_bytes()).hexdigest(),flush=True)
