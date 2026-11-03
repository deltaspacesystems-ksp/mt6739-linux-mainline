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
old = (base / 'mfw_v60.img').read_bytes()
new = (base / 'mfw_v61.img').read_bytes()
oa, ok, old_rd = payload(old)
na, nk, new_rd = payload(new)
oi, od = unpack(ok)
ni, nd = unpack(nk)
assert oa == na and old_rd == new_rd and old[64:576] == new[64:576]
assert b'pvrsrvkm.enable=1' not in new[64:576]
assert ni == (base / 'out/Image_mfw_v61').read_bytes() and ni != oi
assert nd == (base / 'out/v19.dtb').read_bytes()
assert len(ni) < 14 * 1024**2 and len(new) < 24 * 1024**2
assert nd == od, 'DT must be byte-identical to v60'
assert (base / 'export/config-7.3-mfw-v60').read_bytes() == (base / 'export/config-7.3-mfw-v61').read_bytes()
for marker in (b'Native GPU binding disabled', b'mfw-pvr-release', b'GPU fault: power context pinned until reboot', b'continuous video scanout; manual kicks disabled'):
    assert marker in ni, marker
digest = hashlib.sha1()
for blob in (nk, new_rd, b''):
    digest.update(blob); digest.update(struct.pack('<I', len(blob)))
assert new[576:596] == digest.digest()
result = {'boot_sha256':hashlib.sha256(new).hexdigest(), 'boot_bytes':len(new), 'image_bytes':len(ni), 'status':'PASS', 'scope':'native GPU default-disabled; no rendering claim', 'dt_delta':'none, identical v60', 'config_delta':'none, identical v60', 'initramfs_addresses_cmdline':'identical v60'}
(base / 'out/mfw-v61-validation.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result))
