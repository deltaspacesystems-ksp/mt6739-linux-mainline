from pathlib import Path
import struct,zlib,hashlib
w=Path(r"D:\y5_linux_work")
old=(w/'mfw_v58.img').read_bytes();new=(w/'mfw_v59.img').read_bytes()
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
assert ni==(w/'out/Image_mfw_v59').read_bytes() and ni!=oi
assert nd==(w/'out/v18.dtb').read_bytes()
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
import json
op=props(od);np=props(nd);h=json.loads((w/'out/v59-dt-handles.json').read_text())
added_paths=['/pwrap@1000d000/vcore-readonly','/mfw-mfg-probe-clock@10000000','/mfw-gpu-id-probe@10006000']
new_props={(p,n):v for (p,n),v in np.items() if p in added_paths}
assert new_props[('/pwrap@1000d000/vcore-readonly','phandle')]==struct.pack('>I',h['vcore'])
assert new_props[('/mfw-gpu-id-probe@10006000','vcore-supply')]==struct.pack('>I',h['vcore'])
assert new_props[('/mfw-gpu-id-probe@10006000','clocks')]==struct.pack('>I',h['clock'])
assert new_props[('/mfw-gpu-id-probe@10006000','microfw,pmic-provider')]==struct.pack('>I',h['pmic'])
expected={
 added_paths[0]:{'phandle','regulator-name'},
 added_paths[1]:{'compatible','reg','phandle','#clock-cells'},
 added_paths[2]:{'compatible','reg','clocks','vcore-supply','microfw,pmic-provider'},
}
for path,names in expected.items():assert {n for (p,n) in new_props if p==path}==names
for path,bases in [(added_paths[1],[0x10000000,0x1000c000,0x10006000]),(added_paths[2],[0x10006000,0x10001000,0x1020e000])]:
 assert new_props[(path,'reg')]==b''.join(struct.pack('>4I',0,base,0,4096) for base in bases)
np={k:v for k,v in np.items() if k not in new_props}
if h['pmic_phandle_added']:assert np.pop(('/pwrap@1000d000','phandle'))==struct.pack('>I',h['pmic'])
assert np==op,'Unexpected existing DT property change'
for marker in [b'GPU power/ID probe ready',b'VCORE readonly regulator registered',b'MFG CCF probe clock registered']:
 assert marker in ni,marker
assert (w/'export/config-7.3-mfw-v58').read_bytes()==(w/'export/config-7.3-mfw-v59').read_bytes()
assert len(new)<24*1024*1024 and len(ni)<14*1024*1024
digest=hashlib.sha1()
for blob in (nk,nroot,b''):digest.update(blob);digest.update(struct.pack('<I',len(blob)))
assert new[576:596]==digest.digest()
print('PASS: kernel/DT payload, DT delta only readonly VCORE/CCF clock/explicit GPU-ID probe nodes, identical config/initramfs/addresses/cmdline, valid boot SHA1')
print('SHA256',hashlib.sha256(new).hexdigest(),'boot_bytes',len(new),'Image_bytes',len(ni))
