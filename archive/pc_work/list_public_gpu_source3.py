from urllib.request import urlopen,Request
import json
base='https://api.github.com/repos/Power535/android_kernel_common_MT6763/contents/'
for path in ['drivers/misc/mediatek/gpu/gpu_rgx','drivers/misc/mediatek/base/power/mt6739']:
 items=json.load(urlopen(Request(base+path+'?ref=master',headers={'User-Agent':'MICROFW-source-research'}),timeout=30))
 for p in items:print(p['type'],p['path'])
