"""Fetch exact missing PC build dependencies from the official Arch archive.
Run inside WSL archlinux; pacman subsequently verifies cached package integrity/signatures.
This is for the recorded repository snapshot, not an instruction to upgrade the system.
"""
from pathlib import Path
import urllib.request
root=Path('/var/cache/pacman/pkg')
packages=[('g/glycin','glycin-2.2.3-1-x86_64.pkg.tar.zst'),('l/librsvg','librsvg-2:2.63.3-1-x86_64.pkg.tar.zst')]
for folder,name in packages:
 for suffix in ('','.sig'):
  dest=root/(name+suffix)
  if dest.exists():continue
  url='https://archive.archlinux.org/packages/'+folder+'/'+name+suffix
  blob=urllib.request.urlopen(url,timeout=30).read(8*1024*1024)
  if not suffix:assert blob[:4]==b'\x28\xb5\x2f\xfd'
  else:assert 40<len(blob)<4096
  dest.write_bytes(blob)
  print('Cached exact archived dependency',dest.name,len(blob))
