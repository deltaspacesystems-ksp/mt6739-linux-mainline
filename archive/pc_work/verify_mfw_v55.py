from pathlib import Path
import struct,zlib,hashlib
w=Path(r'D:\y5_linux_work')
old=(w/'mfw_v54.img').read_bytes();new=(w/'mfw_v55.img').read_bytes()
def payload(b):
 assert b[:8]==b'ANDROID!'
 ks,ka,rs,ra,ss,sa,ta,page=struct.unpack_from('<8I',b,8)
 assert page==2048 and ss==0
 ro=page+(ks+page-1)//page*page
 return (ka,ra,ta),b[page:page+ks],b[ro:ro+rs]
oa,ok,oroot=payload(old);na,nk,nroot=payload(new)
assert oa==na and oroot==nroot and old[64:576]==new[64:576]
def decompress(k):
 d=zlib.decompressobj(31);image=d.decompress(k)+d.flush();return image,d.unused_data
oi,od=decompress(ok);ni,nd=decompress(nk)
assert oi==ni==(w/'out/Image_mfw_v54').read_bytes()
assert nd==(w/'out/v15.dtb').read_bytes()
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
op={(path.replace('/framebuffer@771e0000','/framebuffer@775eb000'),name):v for (path,name),v in op.items()}
assert op.keys()==np.keys()
diff=[key for key in op if op[key]!=np[key]]
assert diff==[('/framebuffer@775eb000','reg')],diff
assert np[diff[0]]==struct.pack('>4I',0,0x775eb000,0,0x3f4800)
assert 0x771c0000<=0x775eb000 and 0x775eb000+0x3f4800<=0x78000000
digest=hashlib.sha1()
for blob in (nk,nroot,b''):digest.update(blob);digest.update(struct.pack('<I',len(blob)))
assert new[576:596]==digest.digest() and len(new)<24*1024*1024
print('PASS: same #40 Image/initramfs/cmdline/addresses; sole DT property change is measured framebuffer base; boot SHA1 valid')
print('SHA256',hashlib.sha256(new).hexdigest(),'bytes',len(new))
