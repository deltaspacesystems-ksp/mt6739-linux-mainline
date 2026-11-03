from pathlib import Path
import struct,json,hashlib
b=Path('D:/y5_linux_work');p=b/'private_gpu_reference/libusc.so';data=p.read_bytes();shoff=struct.unpack_from('<Q',data,40)[0];entsize,num,namesidx=struct.unpack_from('<HHH',data,58);assert entsize==64
rows=[struct.unpack_from('<IIQQQQIIQQ',data,shoff+i*entsize) for i in range(num)];stringtab=rows[namesidx];strings=data[stringtab[4]:stringtab[4]+stringtab[5]]
sections=[]
for x in rows:
 name=strings[x[0]:].split(b'\0',1)[0].decode();sections.append({'name':name,'offset':x[4],'bytes':x[5]})
patterns={'packed_bvnc16':struct.pack('<4H',16,21,49,22),'packed_bnc16':struct.pack('<4H',16,21,0,22),'bvnc32':struct.pack('<4I',22,49,21,16),'bvnc_text':b'22.49.21.16','bnc_text':b'22.V.21.16'}
results={}
for name,pattern in patterns.items():
 offsets=[];at=0
 while True:
  at=data.find(pattern,at)
  if at<0:break
  offsets.append(hex(at));at+=1
 results[name]=offsets
meta={'method':'Static ELF section inventory and exact target-pattern search, not execution','sha256':hashlib.sha256(data).hexdigest(),'symbols_or_debug_sections':[s['name'] for s in sections if any(w in s['name'] for w in ['symtab','debug','dynsym'])],'target_patterns':results,'feature_values_recovered':False}
(b/'out/usc-static-scan.json').write_text(json.dumps(meta,indent=2)+'\n');print(json.dumps(meta))
