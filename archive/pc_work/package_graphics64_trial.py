"""Pack owned native graphics candidates; never submit a GPU job."""
from pathlib import Path
import hashlib,json,struct
b=Path('D:/y5_linux_work/out');dest=b/'graphics64_trial1';dest.mkdir(exist_ok=True)
manifest={'status':'OFFLINE_CANDIDATE_NOT_RENDER_PROOF','output_gpu_va':'65c0000000','width':64,'height':64,'files':{},'placements':{}}
def raw(n,size=None,sha=None):
 x=(b/n).read_bytes()
 if size is not None:assert len(x)==size,(n,len(x),size)
 if sha:assert hashlib.sha256(x).hexdigest()==sha,n
 return x
def image(name,parts):
 result=bytearray(4096);used=[]
 for off,label,data in parts:
  assert off>=0 and off+len(data)<=4096
  assert all(off+len(data)<=a or off>=z for a,z in used),(name,label)
  used.append((off,off+len(data)));result[off:off+len(data)]=data
  manifest['placements'].setdefault(name,[]).append({'offset':off,'name':label,'bytes':len(data)})
 (dest/name).write_bytes(result)
image('usc-main.bin',[(0,'VS',raw('pco-graphics20-vs.bin',176,'0aa5b981e894dfa6964784bf932ffbf931c06a53d6771777e0d2fd378df42f84')),(0x400,'FS',raw('pco-graphics20-fs.bin',440,'90c2cde42b3385bfa2a4952a011deebc2033bc6c0a4ae79ee655668d283ae14f')),(0x800,'EOT_output0',raw('graphics-eot-output0.bin',40)),(0xc00,'VDM_sync_NOP',raw('vs_nop_common-bounded.bin',8,'164e04488963dec22d26cbc213972940ebc63053ef73eab2ef47d1f98b9e94b5'))])
image('usc-context.bin',[(0x200,'SR_store',raw('cs_store_sr_1024_common-bounded.bin',568,'f43db25cadd502378588c159c7f976c1a809475face4b54d277f890835f4430e')),(0x600,'SR_load',raw('cs_load_sr_256_common-bounded.bin',488,'4d1c683dd275035cafbc37019c3834803ede322f220ef70c0e5285e95c6a57d9'))])
term=raw('graphics-termination.bin',32)
image('general-vdm.bin',[(0,'PPP25_reference',term[:8]),(8,'VDM_draw3',raw('graphics-vdm-segment.bin',32)),(0x100,'VDM_terminate_stream',term[20:])])
image('general-ppp.bin',[(0,'PPP25',raw('graphics-ppp.bin',100)),(0x100,'PPP_final_terminate',term[8:20])])
for src,n,size in [('graphics-main-pds.bin','pds-main.bin',4096),('graphics-context-pds.bin','pds-context.bin',4096),('ta-context112-candidate.bin','ta-context.bin',112),('graphics-pbe-output0.bin','pbe-state.bin',32)]:
 (dest/n).write_bytes(raw(src,size))
assert struct.unpack('<2I',(dest/'pbe-state.bin').read_bytes()[:8])==(0x70000000,0x82019)
for p in sorted(dest.glob('*.bin')):manifest['files'][p.name]={'bytes':p.stat().st_size,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}
manifest['limits']='No TA/3D executed. PPP clip/termination compatibility and complete firmware/fence/reference semantics require controlled validation. VDM5outputs,FSdiscard/punchthrough,phase0. All shader data native open Mesa; no Bionic/HAL.'
(dest/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('GRAPHICS64_BUNDLE_PASS files',len(manifest['files']),'nonoverlap/size/hash guards; NOT rendering')
