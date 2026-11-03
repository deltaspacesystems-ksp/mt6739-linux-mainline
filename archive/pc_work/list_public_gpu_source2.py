from urllib.request import urlopen,Request
import json
for path in ['drivers/misc/mediatek/gpu','drivers/misc/mediatek/base/power']:
 url='https://api.github.com/repos/Power535/android_kernel_common_MT6763/contents/'+path+'?ref=master'
 try:items=json.load(urlopen(Request(url,headers={'User-Agent':'MICROFW-source-research'}),timeout=30))
 except Exception as e:print(path,type(e).__name__);continue
 for p in items:print(p['type'],p['path'])
