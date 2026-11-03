"""Pack v62 using validated v61 boot layout; never writes a phone."""
from pathlib import Path
import gzip,hashlib,json,struct
base=Path('D:/y5_linux_work')
old=(base/'mfw_v61.img').read_bytes()
assert old[:8]==b'ANDROID!'
ks,ka,rs,ra,ss,sa,ta,page=struct.unpack_from('<8I',old,8)
assert page==2048 and ss==0
align=lambda n:(n+page-1)//page*page
roff=page+align(ks)
rd=old[roff:roff+rs]
tail=old[roff+align(rs):]
image=(base/'out/Image_mfw_v62').read_bytes()
dtb=(base/'out/v19.dtb').read_bytes()
assert len(image)<16*1024**2, 'Image unexpectedly large: review before use'
assert image[56:60]==b'ARM\x64'
kernel=gzip.compress(image,compresslevel=9,mtime=0)+dtb
header=bytearray(old[:page])
struct.pack_into('<I',header,8,len(kernel))
h=hashlib.sha1()
for blob in (kernel,rd,b''):
 h.update(blob);h.update(struct.pack('<I',len(blob)))
header[576:596]=h.digest()
pad=lambda b:b+b'\0'*(align(len(b))-len(b))
new=bytes(header)+pad(kernel)+pad(rd)+tail
assert len(new)<24*1024**2
assert header[12:576]==old[12:576]
assert new[page:page+len(kernel)]==kernel
assert new[page+align(len(kernel)):page+align(len(kernel))+rs]==rd
result={'status':'PASS','image_bytes':len(image),'boot_bytes':len(new),'boot_sha256':hashlib.sha256(new).hexdigest(),'dt_ramdisk_cmdline_addresses':'byte-identical v61','scope':'Kexec/file-load, CPU hotplug, loadable hardware framework modules; existing native GPU retained','boot_written':False,'rebooted':False,'hardware_functionality':'not established by configuration or build'}
(base/'mfw_v62.img').write_bytes(new)
(base/'out/mfw-v62-validation.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result))
