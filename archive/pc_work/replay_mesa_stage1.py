from pathlib import Path
import subprocess,re,hashlib,json
root=Path('/home/y5/mesa-native');scratch=Path('/home/y5/mesa-stage1-replay');scratch.mkdir(exist_ok=True)
commit='2736dd1220d58192f5591ca6f9fcc5e60ee368ad'
patches=[Path('/mnt/d/y5_linux_work/out/mesa-llvm23-build.patch'),Path('/mnt/d/y5_linux_work/out/mesa-ddk19-stage1.patch')]
files=[]
for patch in patches:
 for line in patch.read_text().splitlines():
  if line.startswith('+++ b/'):
   name=line[6:];files.append(name);target=scratch/name;target.parent.mkdir(parents=True,exist_ok=True)
   result=subprocess.run(['git','-C',str(root),'show',commit+':'+name],capture_output=True)
   if result.returncode==0:target.write_bytes(result.stdout)
for patch in patches:
 subprocess.run(['git','apply','--check',str(patch)],cwd=scratch,check=True)
 subprocess.run(['git','apply',str(patch)],cwd=scratch,check=True)
for name in files:assert (scratch/name).read_bytes()==(root/name).read_bytes(),name
print('PASS patch replay: exact byte match for',len(files),'Mesa source files')
Path('/mnt/d/y5_linux_work/out/mesa-stage1-replay.json').write_text(json.dumps({'commit':commit,'files':files,'patch_replay':'PASS exact byte match','patches':[{'name':p.name,'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in patches]},indent=2)+'\n')
