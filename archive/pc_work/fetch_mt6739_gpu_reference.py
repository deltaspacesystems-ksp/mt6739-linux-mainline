from pathlib import Path
from urllib.request import urlopen,Request
import json,hashlib
repo='Power535/android_kernel_common_MT6763';root=Path(r'D:\y5_linux_work\sources\mt6739-gpu-reference');root.mkdir(parents=True,exist_ok=True)
def get(url):return urlopen(Request(url,headers={'User-Agent':'MICROFW-source-research'}),timeout=45).read()
sha=json.loads(get('https://api.github.com/repos/'+repo+'/git/ref/heads/master'))['object']['sha']
paths=['drivers/clk/mediatek/clk-mt6739.c','drivers/clk/mediatek/clk-mt6739-pg.c','drivers/misc/mediatek/base/power/mt6739/mtk_gpufreq.c','drivers/misc/mediatek/base/power/mt6739/mtk_gpufreq.h']
manifest={'repository':'https://github.com/'+repo,'commit':sha,'files':[]}
for path in paths:
 url='https://raw.githubusercontent.com/'+repo+'/'+sha+'/'+path
 try:data=get(url)
 except Exception as e:print('NOT_FETCHED',path,type(e).__name__);continue
 p=root/Path(path).name;p.write_bytes(data)
 manifest['files'].append({'path':path,'url':url,'sha256':hashlib.sha256(data).hexdigest(),'bytes':len(data)})
 print('FETCHED',path,len(data))
(root/'provenance.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('PINNED_COMMIT',sha)
