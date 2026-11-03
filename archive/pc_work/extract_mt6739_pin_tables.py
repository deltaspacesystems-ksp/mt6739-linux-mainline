"""Extract fixed GPIO register tables from stock kernel CODE, never partitions.

Only DO/DI/DIR labels are verified against k44_hardware_next.c. Other tables
remain named by structure offset until their callers have been checked.
"""
from pathlib import Path
import hashlib
import json
import struct

work=Path(r'D:\y5_linux_work')
blob=(work/'out/stock_Image').read_bytes()
base=0xffffff8008080000
soc=0xffffff8008bac100
assert blob[56:60]==b'ARM\x64'

def unpack(fmt,address):
    off=address-base
    assert 0 <= off <= len(blob)-struct.calcsize(fmt)
    return struct.unpack_from(fmt,blob,off)

npins=unpack('<I',soc+8)[0]
nbanks=unpack('<I',soc+0x214)[0]
assert npins==169 and nbanks==8
slots={0x30:'unverified_030',0x40:'unverified_040',0x50:'unverified_050',
       0x80:'unverified_080',0x90:'unverified_090',0xa0:'output',0xb0:'input',
       0xc0:'direction',0xe0:'unverified_0e0',0xf0:'unverified_0f0',
       0x100:'unverified_100',0x110:'unverified_110'}
result=dict(source='stock_Image kernel code, not live hardware',
            source_sha256=hashlib.sha256(blob).hexdigest(),npins=npins,regmap_count=nbanks,
            soc_data_va=hex(soc),tables={})
for offset,name in slots.items():
    pointer=unpack('<Q',soc+offset)[0]
    count=unpack('<I',soc+offset+8)[0]
    assert 0<count<=npins,(name,count)
    if name.startswith('unverified_'):
        result['tables'][name]=dict(pointer_va=hex(pointer),count=count,
                                   layout='not decoded: caller/record layout requires verification')
        continue
    rows=[]
    for i in range(count):
        pin,reg,bit,width,bank,pad=unpack('<IIBBBB',pointer+i*12)
        assert pin<npins and reg<0x1000 and bank<nbanks and 1<=width<=32 and bit+width<=32,(name,i,pin,hex(reg),bit,width,bank,pad)
        assert pad==0
        rows.append(dict(pin=pin,offset=reg,bit=bit,width=width,regmap_index=bank))
    assert len({r['pin'] for r in rows})==count
    result['tables'][name]=dict(pointer_va=hex(pointer),count=count,fields=rows)
out=work/'out/mt6739-pin-tables.json'
out.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
for name,table in result['tables'].items():
    print(name,'count',table['count'],
          ('first '+str(table['fields'][0])+' last '+str(table['fields'][-1])) if 'fields' in table else table['layout'])
print('PASS: fixed table bounds, unique pins, register masks and bank indices; no device access')
