from pathlib import Path
import base64
import hashlib
import json
import urllib.request
commit='2736dd1220d58192f5591ca6f9fcc5e60ee368ad'
base='https://chromium.googlesource.com/external/gitlab.freedesktop.org/mesa/mesa/+/'+commit+'/'
root=Path('D:/y5_linux_work/sources/mesa-device-reference')
root.mkdir(parents=True,exist_ok=True)
def get(path,fmt):
    with urllib.request.urlopen(base+path+'?format='+fmt,timeout=45) as r: return r.read()
path='src/imagination/common/'
listing=json.loads(get(path,'JSON').decode().split('\n',1)[1])
(root/'common-tree.json').write_text(json.dumps(listing,indent=2)+'\n')
records=[]
for entry in listing['entries']:
    if entry['name'] not in ('pvr_device_info.c','pvr_device_info.h'): continue
    data=base64.b64decode(get(path+entry['name'],'TEXT'))
    assert hashlib.sha1(b'blob '+str(len(data)).encode()+b'\0'+data).hexdigest()==entry['id']
    (root/entry['name']).write_bytes(data)
    records.append({'path':path+entry['name'],'git_blob':entry['id']})
(root/'provenance.json').write_text(json.dumps({'commit':commit,'mirror':base,'files':records},indent=2)+'\n')
listing2=json.loads(get(path+'device_info/','JSON').decode().split('\n',1)[1])
for entry in listing2['entries']:
    if entry['name'] != 'ge8300.h': continue
    relative=path+'device_info/'+entry['name']
    data=base64.b64decode(get(relative,'TEXT'))
    assert hashlib.sha1(b'blob '+str(len(data)).encode()+b'\0'+data).hexdigest()==entry['id']
    (root/entry['name']).write_bytes(data)
    records.append({'path':relative,'git_blob':entry['id']})
(root/'provenance.json').write_text(json.dumps({'commit':commit,'mirror':base,'files':records},indent=2)+'\n')
print('Mesa common entries:', ', '.join(e['name'] for e in listing['entries'] if 'device' in e['name']))
print('Verified files:',len(records))
