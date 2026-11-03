from pathlib import Path
from elftools.elf.elffile import ELFFile
import json,hashlib
p=Path('/mnt/d/y5_linux_work/private_gpu_reference/rgx.fw.signed');raw=p.read_bytes();assert hashlib.sha256(raw).hexdigest()=='8a843eb1678c0e2fd630c66d4a8070a0a6d4ca5ba7bfd16904d89d66f9fa074c'
def name(d):return d.attributes.get('DW_AT_name').value.decode(errors='replace') if 'DW_AT_name' in d.attributes else ''
def resolved(d):
 while d.tag in ['DW_TAG_typedef','DW_TAG_const_type','DW_TAG_volatile_type'] and 'DW_AT_type' in d.attributes:d=d.get_DIE_from_attribute('DW_AT_type')
 return d
wanted=['RGXFWIF_CMDTA','RGXFWIF_CMD3D','RGXFWIF_TAREGISTERS','RGXFWIF_3DREGISTERS','RGXFWIF_TAREGISTERS_CSWITCH','RGXFWIF_3DREGISTERS_CSWITCH','RGXFWIF_TA_3D_SHARED','RGXFWIF_HWRTDATA','RGXFWIF_FREELIST']
records={}
with p.open('rb') as f:
 dwarf=ELFFile(f).get_dwarf_info()
 for cu in dwarf.iter_CUs():
  for die in cu.iter_DIEs():
   n=name(die)
   if n not in wanted:continue
   target=resolved(die);size=target.attributes.get('DW_AT_byte_size');members=[]
   for m in target.iter_children():
    if m.tag!='DW_TAG_member':continue
    loc=m.attributes.get('DW_AT_data_member_location');typ=resolved(m.get_DIE_from_attribute('DW_AT_type')) if 'DW_AT_type' in m.attributes else None
    members.append({'name':name(m),'offset':loc.value if loc else None,'type':name(typ) if typ else None,'bytes':typ.attributes['DW_AT_byte_size'].value if typ and 'DW_AT_byte_size' in typ.attributes else None})
   entry={'bytes':size.value if size else None,'members':members}
   if n in records:assert records[n]==entry,(n,'inconsistent CU types')
   records[n]=entry
result={'scope':'Exact loaded stock firmware DWARF type facts, no proprietary code/binary copied into repository','firmware_sha256':hashlib.sha256(raw).hexdigest(),'types':records}
Path('/mnt/d/y5_linux_work/gpu-exact-firmware-render-dwarf.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:{'bytes':v['bytes'],'members':len(v['members'])} for k,v in records.items()},indent=2))
