from pathlib import Path
w=Path(r'D:\y5_linux_work')
s=(w/'verify_mfw_v55.py').read_text();a=s.index('def props(');b=s.index('op=props(',a);props=s[a:b]
head='''from pathlib import Path
import struct,zlib,hashlib
w=Path(r"D:\\y5_linux_work")
old=(w/'mfw_v56.img').read_bytes();new=(w/'mfw_v57.img').read_bytes()
def payload(b):
 assert b[:8]==b'ANDROID!'
 ks,ka,rs,ra,ss,sa,ta,page=struct.unpack_from('<8I',b,8)
 assert page==2048 and ss==0
 ro=page+(ks+page-1)//page*page
 return (ka,ra,ta),b[page:page+ks],b[ro:ro+rs]
def decompress(k):
 d=zlib.decompressobj(31);image=d.decompress(k)+d.flush();return image,d.unused_data
oa,ok,oroot=payload(old);na,nk,nroot=payload(new)
assert oa==na and oroot==nroot and old[64:576]==new[64:576]
oi,od=decompress(ok);ni,nd=decompress(nk)
assert ni==(w/'out/Image_mfw_v57').read_bytes() and ni!=oi
assert nd==(w/'out/v16.dtb').read_bytes()
assert b'LK scanout observer IRQ=' in ni
assert b'continuous video scanout; manual kicks disabled' in ni
assert (w/'export/config-7.3-mfw-v56').read_bytes()==(w/'export/config-7.3-mfw-v57').read_bytes()
'''
tail='''op=props(od);np=props(nd)
node='/disp_rdma0@1400b000'
assert np.pop((node,'microfw,observe-lk-scanout'))==b''
assert np[(node,'compatible')]==b'microfw,mt6739-rdma-observer\\0'
np[(node,'compatible')]=b'mediatek,disp_rdma0\\0'
assert op==np, 'Unexpected DT change'
assert np[(node,'interrupts')]==struct.pack('>3I',0,202,8)
assert np[(node,'reg')]==struct.pack('>4I',0,0x1400b000,0,4096)
assert len(new)<24*1024*1024 and len(ni)<14*1024*1024
digest=hashlib.sha1()
for blob in (nk,nroot,b''):digest.update(blob);digest.update(struct.pack('<I',len(blob)))
assert new[576:596]==digest.digest()
print('PASS: kernel/DT payload, exact DT delta only RDMA compatible+opt-in, identical config/initramfs/addresses/cmdline, valid boot SHA1')
print('SHA256',hashlib.sha256(new).hexdigest(),'boot_bytes',len(new),'Image_bytes',len(ni))
'''
(w/'verify_mfw_v57.py').write_text(head+props+tail,newline='\n')
(w/'export_v57_patch.py').write_text('''from pathlib import Path,PurePosixPath
import re,tarfile,difflib
w=Path('/mnt/d/y5_linux_work');k=Path('/home/y5/linux-7.3')
text=(w/'export/mfw-kernel-7.3-v56.patch').read_text()
headers=re.findall(r'^--- (a/[^\\n]+|/dev/null)\\n\\+\\+\\+ b/([^\\n]+)',text,re.M)
assert len(headers)==22
headers.append(('/dev/null','drivers/video/mfw-rdma-observer.c'))
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
(w/'export/mfw-kernel-7.3-v57.patch').write_text(''.join(out),newline='\\n')
print('Exported complete23-file patch from pristine7.3 originals')
''',newline='\n')
s=(w/'check_v56_patch.py').read_text().replace('v56','v57').replace('==22','==23').replace('complete22','complete23');(w/'check_v57_patch.py').write_text(s,newline='\n')
print('Prepared image+exact DT validator and aggregate patch export/dry-run')
