from pathlib import Path
import json,urllib.request,hashlib
root=Path('D:/y5_linux_work/sources/mt6739-connectivity-reference');tree=json.loads((root/'mt6739-tree.json').read_text());repo='parthibx24/android_kernel_wiko_p200';commit='096323c2d5db8a05e84df31608fc0c9d22e47b31';prefix='drivers/misc/mediatek/videocodec/mt6739/'
records=[]
for e in tree['tree']:
 if e['type']!='blob':continue
 path=prefix+e['path'];url=f'https://raw.githubusercontent.com/{repo}/{commit}/{path}'
 with urllib.request.urlopen(url,timeout=40) as r:data=r.read()
 assert hashlib.sha1(b'blob '+str(len(data)).encode()+b'\0'+data).hexdigest()==e['sha']
 target=root/'vcodec'/e['path'];target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
 records.append({'path':path,'git_blob':e['sha'],'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
manifest={'repository':'https://github.com/'+repo,'commit':commit,'files':records,'confidence':'MT6739 public reference, not exact Huawei source proof'}
(root/'vcodec-provenance.json').write_text(json.dumps(manifest,indent=2)+'\n');print('Verified',len(records),'reference files')
