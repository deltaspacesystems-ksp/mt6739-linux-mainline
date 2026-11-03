"""Install verified public source + overlay into the known v59 kernel tree."""
from pathlib import Path
import hashlib
import json
import shutil
k = Path('/home/y5/linux-7.3')
base = Path('/mnt/d/y5_linux_work')
port = Path('/home/y5/rogue-ddk19-port')
dest = k / 'drivers/gpu/drm/mfw-rogue-ddk19'
assert (k / '.config').read_bytes() == (base / 'export/config-7.3-mfw-v59').read_bytes(), 'Unexpected config drift'
assert not dest.exists(), 'Refuse to overwrite an existing kernel driver directory'
backup = base / 'export/v59-before-ddk-integration'
backup.mkdir(exist_ok=True)
saved = {}
for relative in ('.config', 'drivers/gpu/drm/Makefile', 'drivers/gpu/drm/Kconfig'):
    data = (k / relative).read_bytes()
    path = backup / relative
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists():
        assert path.read_bytes() == data, 'Existing backup differs: ' + relative
    else:
        path.write_bytes(data)
    saved[relative] = hashlib.sha256(data).hexdigest()
manifest = json.loads((base / 'out/ddk19-source-tree.json').read_text())
paths = [entry['path'] for entry in manifest['tree']['tree'] if entry['type'] == 'blob']
paths += ['services/server/env/linux/mfw-dmabuf-fill.h', 'include/mfw-linux-compat.h', 'include/mfw-ddk-power.h', 'include/mfw-mfg-power-sequence.h', 'services/system/rgx_mtk/mfw-ddk-power.c']
for relative in paths:
    target = dest / relative
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(port / relative, target)
p = k / 'drivers/gpu/drm/Makefile'
with p.open('a') as f:
    f.write('\nobj-$(CONFIG_DRM_MFW_ROGUE_DDK19) += mfw-rogue-ddk19/\n')
p = k / 'drivers/gpu/drm/Kconfig'
with p.open('a') as f:
    f.write('''
config DRM_MFW_ROGUE_DDK19
\tbool "MICROFW MT6739 native Rogue DDK 1.9 bringup"
\tdepends on DRM && ARM64 && ARCH_MEDIATEK && OF && COMMON_CLK && REGULATOR
\thelp
\t  Experimental native kernel port for BVNC 22.49.21.16. Binding is
\t  disabled by default. Explicit enable and a controlled firmware test
\t  are required after userspace has mounted its root filesystem.
''')
with (k / '.config').open('a') as f:
    f.write('\nCONFIG_DRM_MFW_ROGUE_DDK19=y\n')
(backup / 'sha256.json').write_text(json.dumps(saved, indent=2) + '\n')
print('Installed', len(paths), 'source files; v59 config/Makefile/Kconfig backups verified')
