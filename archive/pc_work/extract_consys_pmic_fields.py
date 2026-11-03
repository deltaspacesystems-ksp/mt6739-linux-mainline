from pathlib import Path
import struct,json,hashlib
b=Path('D:/y5_linux_work');data=(b/'out/stock_Image').read_bytes();base=0xffffff8008080000;table=0xffffff8008bd8390
assert data[56:60]==b'ARM\x64'
rows=[]
for i in [0xc2f,0xc31,0xd03,0xd08,0xd3b,0xd41]:
 off=table+i*12-base;assert 0<=off<=len(data)-12
 index,addr,mask,shift=struct.unpack_from('<IHHB',data,off)
 assert index==i and addr%2==0 and mask and 0<=shift<16 and mask<<shift<=65535
 rows.append(dict(selector=hex(i),register=hex(addr),mask=hex(mask),shift=shift))
r={'source':'stock kernel4.4 executable plus pmic_set_register_value decompilation','table_va':hex(table),'stride':12,'schema':'id u32 at0, addr u16 at4, mask u16 at6, shift u8 at8','fields':rows,'meaning':'Selectors are enum IDs, not PMIC addresses; no hardware writes','confidence':'Exact extracted fields, caller labels in wmt_consys.c; no assumed PMIC feature names'}
(b/'out/consys-pmic-fields.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(rows))
