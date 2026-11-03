from pathlib import Path
import struct,json,hashlib
B=Path(r'D:\y5_linux_work');T=B/'codex_task1';O=B/'scratchpad/phase2_codex'
def u32(b,o):return struct.unpack_from('<I',b,o)[0]
usc=(T/'stock/kick2_vaba00035000.bin').read_bytes()
vs=(T/'ours/usc-main.bin').read_bytes()[:176]
assert not any(usc[0x800:0x800+len(vs)]), 'Chosen stock USC window is not empty'
pds=bytearray((T/'ours/pds-main.bin').read_bytes()[:28]);assert u32(pds,0)==0x10000
struct.pack_into('<I',pds,0,0x35800)
# PDS data at stock ae000, our code immediately after it at ae010.
patch=[]
def add(region,off,value):patch.append('p:2:%x:%x:%x'%(region,off,value))
for i in range(0,len(vs),4):add(0x6f000,0x800+i,u32(vs,i))
for i in range(0,len(pds),4):add(0x96000,i,u32(pds,i))
add(0x8a000,0x10,0xae000);add(0x8a000,0x14,0xae010)
add(0x8a000,0x18,5)
# Preserve stock PPP layout24, stock termination, stock FS/PDS, update only output contract.
assert u32((T/'stock/kick2_va65c03ad000.bin').read_bytes(),0x54)==0x04010000
add(0x99000,0x54,0x05040000)
(O/'hybrid_vs5.patch.txt').write_text(';'.join(patch)+'\n')
(O/'native-vs.bin').write_bytes(vs);(O/'native-vs-pds.bin').write_bytes(pds)
s=(B/'graphics19_replay.c').read_text();assert 'patches[32]' in s and 'npatch < 32' in s
s=s.replace('patches[32]','patches[512]').replace('npatch < 32','npatch < 512')
s=s.replace('printf("patch p FAILED (region %llx)\\n", patches[pi].a);','{ fprintf(stderr,"patch p FAILED (region %llx); abort before kick\\n", patches[pi].a); return 4; }')
s=s.replace('for (int k = 0; k < 16; k++) printf(" %x", w[k]);', 'for (int k = 0; k < 16; k++) { printf(" %x", w[k]); }')
(O/'graphics19_replay_hybrid.c').write_text(s,newline='\n')
manifest={'status':'OFFLINE_CANDIDATE_NOT_SUBMITTED','patch_count':len(patch),'kick_mask':2,'changes':'native VS176 atba00035800 + own PDS at a8000ae000, vertex outputs5, PPP outputselect05040000; retain stock FS, PPP header/layout, termination, VDM allocation word','unresolved':'PSIZE/PPP outputselect contract not fully decoded; test can HWR. Native VS geometry differs from stock, do not require1352px. Replayer originally512-slot copy not canonical replacement.','files':{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in O.glob('*') if p.is_file()}}
(O/'hybrid_manifest.json').write_text(json.dumps(manifest,indent=2))
print('Candidate generated:',len(patch),'word patches; USC zero-window verified; NOT submitted')
