"""Read-only initial PowerVR7.3 firmware header gate; never patches/executes FW.
Passing this gate does not prove firmware ABI, feature or hardware compatibility.
Constants/layout: Linux7.3 pvr_fw_info.h and pvr_fw_validate() initial checks.
"""
import argparse,hashlib,json,struct
from pathlib import Path

def inspect(blob):
 out={'bytes':len(blob),'sha256':hashlib.sha256(blob).hexdigest(),'hardware_id_verified':False}
 if blob[:4]==b'\x7fELF' and len(blob)>=20:
  out['elf_class']=blob[4];out['elf_data']=blob[5]
  if blob[5] in (1,2):out['elf_machine']=struct.unpack_from('<H' if blob[5]==1 else '>H',blob,18)[0]
 if len(blob)<4096 or len(blob)%4096:
  out.update(mainline_header_gate='REJECT',first_reason='size must be at least4096 and divisible by4096');return out
 values=struct.unpack_from('<4IQ2I2H3I',blob,len(blob)-4096)
 names=['info_version','header_len','layout_entry_num','layout_entry_size','bvnc_packed','fw_page_size','flags','fw_major','fw_minor','fw_build','device_info_size','padding']
 header=dict(zip(names,values));out['trailer_candidate']=header
 checks=[(header['info_version']==3,'info_version must equal3'),(header['header_len']==48,'header_len must equal48'),(header['layout_entry_size']==24,'layout_entry_size must equal24'),(header['layout_entry_num']<=8,'layout_entry_num must not exceed8'),(bool(header['flags']&1),'open-source firmware flag required'),(header['fw_major']==1,'supported firmware major version is1')]
 for valid,reason in checks:
  if not valid:out.update(mainline_header_gate='REJECT',first_reason=reason);return out
 out.update(mainline_header_gate='INITIAL_HEADER_PASSES',first_reason='full device/layout/ABI validation still required');return out

def selftest():
 assert inspect(b'')['mainline_header_gate']=='REJECT'
 blob=bytearray(8192);struct.pack_into('<4IQ2I2H3I',blob,4096,3,48,1,24,0,4096,1,1,0,0,0,0)
 assert inspect(blob)['mainline_header_gate']=='INITIAL_HEADER_PASSES'
 struct.pack_into('<I',blob,4096,0x02000000)
 assert inspect(blob)['first_reason']=='info_version must equal3'
 print('PASS: short/unaligned image rejection, initial header gate and stock-style version rejection')
if __name__=='__main__':
 ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('file',nargs='?',type=Path);ap.add_argument('--selftest',action='store_true');a=ap.parse_args()
 if a.selftest:selftest()
 elif a.file:print(json.dumps(inspect(a.file.read_bytes()),indent=2))
 else:ap.error('provide a firmware file or --selftest')
