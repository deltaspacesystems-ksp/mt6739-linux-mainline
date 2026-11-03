"""Static camera power sequences, from verified stock code descriptors.

No enum voltage/GPIO meanings are inferred. Tables may list cameras absent
from this handset; they are not a detected sensor inventory.
"""
from pathlib import Path
import struct, json, hashlib
w=Path(r'D:\y5_linux_work')
b=(w/'out/stock_Image').read_bytes()
base=0xffffff8008080000
assert b[56:60]==b'ARM\x64'
def unpack(fmt,va):
 off=va-base
 assert 0<=off<=len(b)-struct.calcsize(fmt)
 return struct.unpack_from(fmt,b,off)
def name(va):
 off=va-base
 assert 0<=off<len(b)
 end=b.index(b'\0',off,off+100)
 s=b[off:end].decode('ascii')
 assert s and all(c.isalnum() or c in '_-' for c in s),s
 return s
tables={}
for label,va in [('sensor',0xffffff800947ca70)]:
 rows=[]
 for i in range(32):
  entry=va+i*248
  ptr=unpack('<Q',entry)[0]
  if not ptr:break
  steps=[]
  for j in range(12):
   pin,on,on_delay,off,off_delay=unpack('<5i',entry+8+j*20)
   if pin==0:break
   assert -1<=pin<32 and 0<=on_delay<=5000 and 0<=off_delay<=5000
   steps.append(dict(pin_enum=pin,on_state_enum=on,on_delay_ms=on_delay,
                     off_state_enum=off,off_delay_ms=off_delay))
  rows.append(dict(name=name(ptr),descriptor_va=hex(entry),steps=steps))
 else:raise AssertionError('Missing bounded table terminator')
 tables[label]=rows
result=dict(source_sha256=hashlib.sha256(b).hexdigest(),
 platform_table='0xffffff800969bb98 is outside the file-backed image; populated at runtime, not decoded as static data.',
 confidence='Compiled candidates only, not camera identity. Numeric pin/state enums remain unverified.',
 semantics='Power-on traverses forward, sets on state then delays; power-off traverses reverse, delays on_delay_ms then sets off state (caller uses +8, not +16).',
 tables=tables)
(w/'out/camera-power-sequences.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
for label,rows in tables.items():print(label,[(row['name'],len(row['steps'])) for row in rows])
