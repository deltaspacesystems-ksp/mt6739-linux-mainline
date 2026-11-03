from pathlib import Path
import struct,hashlib,json
b=Path('D:/y5_linux_work');x=(b/'mfw_v62.img').read_bytes();ks,_,rs=struct.unpack_from('<3I',x,8);page=struct.unpack_from('<I',x,36)[0];off=page+(ks+page-1)//page*page
rd=x[off:off+rs];(b/'out/kexec-v62-initramfs.gz').write_bytes(rd)
d={}
for p in [b/'out/Image_mfw_v62',b/'out/kexec-v62-initramfs.gz']:d[p.name]={'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}
(b/'out/kexec-v62-inputs.json').write_text(json.dumps(d,indent=2)+'\n');print(json.dumps(d))
