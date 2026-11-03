from pathlib import Path
import hashlib
import importlib.util
import json
import struct
import zlib
base = Path('D:/y5_linux_work')
spec = importlib.util.spec_from_file_location('fdt_props', base / 'out/fdt_props_v60.py')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
props = module.props
def payload(data):
    assert data[:8] == b'ANDROID!'
    ks, ka, rs, ra, ss, sa, ta, page = struct.unpack_from('<8I', data, 8)
    assert page == 2048 and ss == 0
    offset = page + (ks + page - 1) // page * page
    return (ka, ra, ta), data[page:page+ks], data[offset:offset+rs]
def unpack(data):
    inflater = zlib.decompressobj(31)
    image = inflater.decompress(data) + inflater.flush()
    return image, inflater.unused_data
old = (base / 'mfw_v59.img').read_bytes()
new = (base / 'mfw_v60.img').read_bytes()
oa, ok, old_rd = payload(old)
na, nk, new_rd = payload(new)
oi, od = unpack(ok)
ni, nd = unpack(nk)
assert oa == na and old_rd == new_rd and old[64:576] == new[64:576]
assert b'pvrsrvkm.enable=1' not in new[64:576]
assert ni == (base / 'out/Image_mfw_v60').read_bytes() and ni != oi
assert nd == (base / 'out/v19.dtb').read_bytes()
assert len(ni) < 14 * 1024**2 and len(new) < 24 * 1024**2
op, np = props(od), props(nd)
gpu = '/gpu@13000000'
allowed = {'compatible', 'reg', 'reg-names', 'interrupt-parent', 'interrupts', 'interrupt-names', 'clocks', 'vcore-supply', 'microfw,pmic-provider'}
gp = {name:value for (path,name),value in np.items() if path == gpu}
assert set(gp) == allowed
assert gp['compatible'] == b'microfw,mt6739-rogue-ddk19\0'
assert gp['reg'] == b''.join(struct.pack('>4I', 0, addr, 0, size) for addr,size in [(0x13000000,0x80000),(0x10006000,4096),(0x10001000,4096),(0x1020e000,4096)])
assert gp['interrupt-parent'] == struct.pack('>I', 1)
assert gp['interrupts'] == struct.pack('>3I', 0, 226, 8)
handles = json.loads((base / 'out/v59-dt-handles.json').read_text())
for prop, name in [('clocks','clock'), ('vcore-supply','vcore'), ('microfw,pmic-provider','pmic')]:
    assert gp[prop] == struct.pack('>I', handles[name])
np = {key:value for key,value in np.items() if key[0] != gpu}
assert np.pop(('/mfw-gpu-id-probe@10006000','status')) == b'disabled\0'
assert np == op, 'Unexpected existing DT change'
def config(name):
    return {line.split('=',1)[0]:line for line in (base / ('export/config-7.3-mfw-' + name)).read_text().splitlines() if line.startswith('CONFIG_') or line.startswith('# CONFIG_')}
oc, nc = config('v59'), config('v60')
assert nc.pop('CONFIG_DRM_MFW_ROGUE_DDK19') == 'CONFIG_DRM_MFW_ROGUE_DDK19=y'
assert nc == oc, 'Unexpected config change'
for marker in (b'Native GPU binding disabled', b'mfw-pvr-release', b'GPU fault: power context pinned until reboot', b'continuous video scanout; manual kicks disabled'):
    assert marker in ni, marker
digest = hashlib.sha1()
for blob in (nk, new_rd, b''):
    digest.update(blob); digest.update(struct.pack('<I', len(blob)))
assert new[576:596] == digest.digest()
result = {'boot_sha256':hashlib.sha256(new).hexdigest(), 'boot_bytes':len(new), 'image_bytes':len(ni), 'status':'PASS', 'scope':'native GPU default-disabled; no rendering claim', 'dt_delta':'new GPU SPI226+low and ID-probe disabled only', 'config_delta':'DRM_MFW_ROGUE_DDK19=y only', 'initramfs_addresses_cmdline':'identical v59'}
(base / 'out/mfw-v60-validation.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result))
