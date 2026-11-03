from urllib.request import urlopen,Request
import json
url='https://api.github.com/repos/Power535/android_kernel_common_MT6763/contents/drivers/gpu?ref=master'
items=json.load(urlopen(Request(url,headers={'User-Agent':'MICROFW-source-research'}),timeout=30))
for p in items:print(p['type'],p['path'])
