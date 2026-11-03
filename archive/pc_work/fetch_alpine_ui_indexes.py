from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import urllib.request,tarfile,io
root=Path(r'D:\y5_linux_work\alpine-ui-packages')
def one(repo):
 p=root/repo/'APKINDEX.tar.gz';p.parent.mkdir(parents=True,exist_ok=True)
 url='https://dl-cdn.alpinelinux.org/alpine/v3.24/'+repo+'/aarch64/APKINDEX.tar.gz'
 b=urllib.request.urlopen(url,timeout=30).read(20*1024*1024);assert b[:2]==b'\x1f\x8b';p.write_bytes(b)
 with tarfile.open(fileobj=io.BytesIO(b),mode='r:gz') as t:text=t.extractfile('APKINDEX').read().decode()
 names={'labwc','foot','wayland-dev','cairo-dev','pango-dev','wayland-protocols','build-base','pkgconf','font-dejavu','ttf-dejavu','seatd','wlroots0.20','wlroots0.19','wlroots'}
 for entry in text.split('\n\n'):
  fields=dict(line.split(':',1) for line in entry.splitlines() if ':' in line)
  if fields.get('P') in names:print(repo,fields['P'],fields['V'],flush=True)
 print(repo,'index bytes',len(b),flush=True)
with ThreadPoolExecutor(max_workers=2) as pool:list(pool.map(one,['main','community']))
