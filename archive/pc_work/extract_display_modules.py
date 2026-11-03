from pathlib import Path
import struct,json,hashlib
w=Path(r'D:\y5_linux_work');b=(w/'out/stock_Image').read_bytes();base=0xffffff8008080000
table=(-0x7ff6b99550)&((1<<64)-1)
def u(fmt,va):
 off=va-base;assert 0<=off<=len(b)-struct.calcsize(fmt)
 return struct.unpack_from(fmt,b,off)[0]
def name(va):
 off=va-base;assert 0<=off<len(b)
 end=b.index(b'\0',off,off+128);return b[off:end].decode('ascii')
rows=[]
for i in range(49):
 va=table+72*i
 rows.append(dict(module=i,name=name(u('<Q',va)),physical_base=hex(u('<Q',va+0x20)),irq_initial=u('<I',va+0x38)))
result=dict(source_sha256=hashlib.sha256(b).hexdigest(),
 evidence='Static stock module descriptors; name/PA/IRQ offsets and count verified against k44_display_modules.c. IRQ field may be populated at runtime, not a DT SPI number.',modules=rows)
(w/'out/display-modules.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
for row in rows:
 if any(x in row['name'].lower() for x in ['ovl','rdma','dsi','mutex']):print(row)
