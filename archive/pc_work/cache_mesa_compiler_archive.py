from pathlib import Path
import urllib.request,urllib.parse
cache=Path('/var/cache/pacman/pkg'); inputs=Path('/mnt/d/y5_linux_work/out/mesa-compiler-package-urls.txt').read_text().splitlines();paths=[]
for line in inputs:
 name,version,url=line.split();filename=Path(urllib.parse.urlparse(url).path).name
 for suffix in ['', '.sig']:
  target=cache/(filename+suffix)
  if not target.exists():
   archive=f'https://archive.archlinux.org/packages/{name[0]}/{name}/{filename}{suffix}'
   with urllib.request.urlopen(archive,timeout=45) as r:data=r.read()
   target.write_bytes(data);print('Archive cached',target.name,len(data))
 paths.append(str(cache/filename))
Path('/mnt/d/y5_linux_work/out/mesa-compiler-package-paths.txt').write_text('\n'.join(paths)+'\n')
