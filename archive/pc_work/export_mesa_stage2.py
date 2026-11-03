from pathlib import Path
import difflib,subprocess,hashlib,json
name='src/imagination/vulkan/winsys/pvrsrvkm/pvr_srv_ddk19.h';root=Path('/home/y5/mesa-native');old=Path('/home/y5/mesa-stage1-replay')/name;new=root/name
patch=''.join(difflib.unified_diff(old.read_text().splitlines(True),new.read_text().splitlines(True),fromfile='a/'+name,tofile='b/'+name))
file=Path('/mnt/d/y5_linux_work/out/mesa-ddk19-stage2-on-stage1.patch');file.write_text(patch)
scratch=Path('/home/y5/mesa-stage2-replay');target=scratch/name;target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(old.read_bytes())
subprocess.run(['git','apply','--check',str(file)],cwd=scratch,check=True);subprocess.run(['git','apply',str(file)],cwd=scratch,check=True);assert target.read_bytes()==new.read_bytes()
print('PASS stage2-on-stage1 exact source replay')
Path('/mnt/d/y5_linux_work/out/mesa-stage2-validation.json').write_text(json.dumps({'status':'PASS','patch':'stage2 applies on stage1, not pristine alone','patch_sha256':hashlib.sha256(file.read_bytes()).hexdigest(),'host_tests':'ASan+UBSan translation/guards/error25 cases PASS','live_phone':'standalone native smoke uses exact Mesa adapter/structs, real DRM ioctl; CONNECT/context/heapcount/create/destroy/DISCONNECT PASS','heap_count':17,'cpu_cache_line':64,'driver':'OK','firmware':'OK','hwr':0,'crr':0,'rendering':False,'scope':'7 implemented operations total; details/reserve/map/PMR/sync/jobs still blocked','full_Mesa_on_phone':False},indent=2)+'\n')
