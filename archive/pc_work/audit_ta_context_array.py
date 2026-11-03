from pathlib import Path
from elftools.elf.elffile import ELFFile
import json
p=Path('/mnt/d/y5_linux_work/private_gpu_reference/rgx.fw.signed')
def name(d):return d.attributes['DW_AT_name'].value.decode() if 'DW_AT_name' in d.attributes else ''
def resolve(d):
 while d.tag in ['DW_TAG_typedef','DW_TAG_const_type','DW_TAG_volatile_type']:d=d.get_DIE_from_attribute('DW_AT_type')
 return d
records={}
with p.open('rb') as f:
 for cu in ELFFile(f).get_dwarf_info().iter_CUs():
  for d in cu.iter_DIEs():
   if name(d)!='RGXFWIF_TAREGISTERS_CSWITCH':continue
   t=resolve(d)
   for m in t.iter_children():
    if name(m)!='asTAState':continue
    a=resolve(m.get_DIE_from_attribute('DW_AT_type'));e=resolve(a.get_DIE_from_attribute('DW_AT_type'))
    rec={'array_element_type':name(e),'element_bytes':e.attributes['DW_AT_byte_size'].value,'bounds':[dict((k,v.value) for k,v in c.attributes.items()) for c in a.iter_children()],'members':[]}
    for c in e.iter_children():
     if c.tag!='DW_TAG_member':continue
     ty=resolve(c.get_DIE_from_attribute('DW_AT_type'));rec['members'].append({'name':name(c),'offset':c.attributes['DW_AT_data_member_location'].value,'bytes':ty.attributes.get('DW_AT_byte_size').value if 'DW_AT_byte_size' in ty.attributes else None})
    records[name(e)]=rec
Path('/mnt/d/y5_linux_work/out/ta-context-element-dwarf.json').write_text(json.dumps(records,indent=2)+'\n');print(json.dumps(records,indent=2))
