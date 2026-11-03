from pathlib import Path
import struct,zlib,hashlib
w=Path(r"D:\y5_linux_work")
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
def props(b):
 magic,total,off,strings_off,_,_,_,_,string_size,size=struct.unpack_from('>10I',b)
 assert magic==0xd00dfeed and total==len(b)
 strings=b[strings_off:strings_off+string_size];p=off;stack=[];out={}
 while p<off+size:
  token=struct.unpack_from('>I',b,p)[0];p+=4
  if token==1:
   end=b.index(0,p);stack.append(b[p:end].decode());p=(end+4)&~3
  elif token==2:stack.pop()
  elif token==3:
   length,no=struct.unpack_from('>II',b,p);p+=8
   end=strings.index(0,no);name=strings[no:end].decode()
   out[('/'+'/'.join(stack[1:]),name)]=b[p:p+length];p=(p+length+3)&~3
  elif token==4:pass
  elif token==9:return out
  else:raise AssertionError(token)
 raise AssertionError('Missing FDT_END')
op=props(od);np=props(nd)
node='/disp_rdma0@1400b000'
assert np.pop((node,'microfw,observe-lk-scanout'))==b''
assert np[(node,'compatible')]==b'microfw,mt6739-rdma-observer\0'
np[(node,'compatible')]=b'mediatek,disp_rdma0\0'
assert op==np, 'Unexpected DT change'
assert np[(node,'interrupts')]==struct.pack('>3I',0,202,8)
assert np[(node,'reg')]==struct.pack('>4I',0,0x1400b000,0,4096)
assert len(new)<24*1024*1024 and len(ni)<14*1024*1024
digest=hashlib.sha1()
for blob in (nk,nroot,b''):digest.update(blob);digest.update(struct.pack('<I',len(blob)))
assert new[576:596]==digest.digest()
print('PASS: kernel/DT payload, exact DT delta only RDMA compatible+opt-in, identical config/initramfs/addresses/cmdline, valid boot SHA1')
print('SHA256',hashlib.sha256(new).hexdigest(),'boot_bytes',len(new),'Image_bytes',len(ni))
