"""Fetch a pinned upstream Mesa reference through Google's public Git mirror."""
from pathlib import Path
import base64
import hashlib
import json
import urllib.request
ROOT = Path('D:/y5_linux_work/sources/mesa-srv-reference')
COMMIT = '2736dd1220d58192f5591ca6f9fcc5e60ee368ad'
URL = 'https://chromium.googlesource.com/external/gitlab.freedesktop.org/mesa/mesa/+/' + COMMIT + '/'
SUBTREE = 'src/imagination/vulkan/winsys/pvrsrvkm/'
def get(path, fmt):
    with urllib.request.urlopen(URL + path + '?format=' + fmt, timeout=45) as response:
        return response.read()
listing = json.loads(get(SUBTREE, 'JSON').decode().split('\n', 1)[1])
ROOT.mkdir(parents=True, exist_ok=True)
(ROOT / 'tree.json').write_text(json.dumps(listing, indent=2) + '\n')
selected = {'pvr_srv.c', 'pvr_srv_bridge.c', 'pvr_srv_bridge.h', 'pvr_srv.h', 'pvr_srv_public.h', 'meson.build'}
records = []
for entry in listing['entries']:
    if entry['name'] not in selected:
        continue
    data = base64.b64decode(get(SUBTREE + entry['name'], 'TEXT'))
    blob = hashlib.sha1(b'blob ' + str(len(data)).encode() + b'\0' + data).hexdigest()
    if blob != entry['id']:
        raise RuntimeError('Git blob mismatch: ' + entry['name'])
    (ROOT / entry['name']).write_bytes(data)
    records.append({'path': SUBTREE + entry['name'], 'git_blob': blob, 'sha256': hashlib.sha256(data).hexdigest()})
(ROOT / 'provenance.json').write_text(json.dumps({'upstream': 'https://gitlab.freedesktop.org/mesa/mesa', 'mirror': URL, 'commit': COMMIT, 'files': records}, indent=2) + '\n')
print('Verified Mesa reference files:', len(records))
