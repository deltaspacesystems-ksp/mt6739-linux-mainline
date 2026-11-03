from pathlib import Path
from elftools.elf.elffile import ELFFile
from elftools.dwarf.locationlists import LocationParser
from elftools.dwarf.descriptions import describe_DWARF_expr
p=Path('/mnt/d/y5_linux_work/private_gpu_reference/rgx.fw.signed')
with p.open('rb') as f:
 dwarf=ELFFile(f).get_dwarf_info();lp=LocationParser(dwarf.location_lists())
 for cu in dwarf.iter_CUs():
  n=cu.get_top_DIE().attributes.get('DW_AT_name')
  if not n or b'rgxfw_ta.c' not in n.value:continue
  for die in cu.iter_DIEs():
   n=die.attributes.get('DW_AT_name')
   if not n or n.value!=b'rgxfw_ta_hwevent':continue
   for child in die.iter_children():
    n=child.attributes.get('DW_AT_name');loc=child.attributes.get('DW_AT_location')
    if n and loc and n.value in [b'psTACmd',b'psCmdHeader']:
     print(n.value)
     for x in lp.parse_from_attribute(loc,cu['version'],die=child):
      if hasattr(x,'loc_expr'):print(hex(x.begin_offset),hex(x.end_offset),describe_DWARF_expr(x.loc_expr,dwarf.structs))


