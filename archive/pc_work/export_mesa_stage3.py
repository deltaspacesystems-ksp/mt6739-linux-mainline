from pathlib import Path
import difflib,subprocess,hashlib,json
base=Path('/mnt/d/y5_linux_work');name='src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h';new=Path('/home/y5/mesa-native')/name;old=base/'out/pvr_srv_ddk19-stage2.h'
patch=''.join(difflib.unified_diff(old.read_text().splitlines(True),new.read_text().splitlines(True),fromfile='a/'+name,tofile='b/'+name))
p=base/'out/mesa-ddk19-stage3-on-stage2.patch';p.write_text(patch)
r=Path('/home/y5/mesa-stage3-replay');t=r/name;t.parent.mkdir(parents=True,exist_ok=True);t.write_bytes(old.read_bytes());subprocess.run(['git','apply','--check',str(p)],cwd=r,check=True);subprocess.run(['git','apply',str(p)],cwd=r,check=True);assert t.read_bytes()==new.read_bytes()
print('PASS stage3-on-stage2 exact replay')
(base/'out/mesa-stage3-validation.json').write_text(json.dumps({'stage':'3a reservation lifecycle','patch_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'host_sanitizers':'PASS exact adapter / modern Mesa structs / old-wire ordering / alignment, null heap, zero size, overflow, size guards, ioctl failure, error translation and RETRY','host_library_build':'PASS x86_64, not a phone Mesa library','phone':'PASS native standalone exact Mesa adapter, real ioctls, context+heap+4096-byte reservation+cleanup','heap_count':17,'cache_line':64,'firmware':'OK','driver':'OK','hwr':0,'crr':0,'reboot':False,'rendering':False,'limitations':['no PMR flag translation yet','no map/sync/job translation','no complete hardware compiler profile']},indent=2)+'\n')
