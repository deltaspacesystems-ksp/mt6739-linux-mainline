from pathlib import Path
import re,struct,json,hashlib
r=Path('/home/y5/mesa-native');s=(r/'build-mfw/src/imagination/pco/uscgen/pco_uscgen_programs.c').read_text()
result={'mesa_commit':'2736dd1220d58192f5591ca6f9fcc5e60ee368ad','status':'offline artifact audit only; no GPU upload','programs':{}}
for name in ['cs_store_sr_1024_common','cs_load_sr_256_common','cs_load_sr_16_common']:
 m=re.search(r'static const uint32_t '+name+r'_0_common\[(\d+)\] = \{(.*?)\};',s,re.S);words=[int(x,16) for x in re.findall(r'0x[0-9a-f]+',m[2])];assert len(words)==int(m[1]);raw=struct.pack('<'+'I'*len(words),*words);size=struct.unpack_from('<H',raw,5)[0];binary=raw[8:];assert len(binary)==size*4
 result['programs'][name]={'temps':raw[0],'vtxins':raw[1],'binary_bytes':len(binary),'binary_sha256':hashlib.sha256(binary).hexdigest()}
result['profile']={'source':'src/imagination/pco/uscgen/pco_clc.c pvr_device_info_common','unified_store_depth':64,'usc_slots':14,'warning':'Generic offline compiler inputs, NOT measured22.49.21.16 limits; common suffix is not target validation'}
result['interface_difference']='Public PDS setup emits DOUTW to provide two 64-bit state pointers to USC vtx inputs; stock primitive only kicks USC with task control. Native public USC requires matching public state/PDS calling convention; do not splice with stock minimal PDS.'
result['legacy_sizes_bytes']={'store':312,'load':232,'note':'Private static reference only. Byte size difference is not proof of incompatible ISA.'}
result['next_gate']='Audit exact DMA/shared-register instructions and target quirks/profile; validate native state-buffer contract and program pairing before any submission.'
Path('/mnt/d/y5_linux_work/gpu-public-context-programs.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
