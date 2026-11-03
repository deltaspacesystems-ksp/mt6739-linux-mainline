"""Export only source metadata, reviewed text overlay and build helpers."""
import difflib
import hashlib
import json
import shutil
from pathlib import Path
base = Path('D:/y5_linux_work')
repo = Path('X:/y5linux')
source = base / 'sources/rogue-ddk19'
port = base / 'rogue-ddk19-port'
target = repo / 'phone/kernel/gpu/ddk19'
target.mkdir(exist_ok=True)
manifest = json.loads((base / 'out/ddk19-source-tree.json').read_text())
patch = []
changed = []
for entry in manifest['tree']['tree']:
    if entry['type'] != 'blob':
        continue
    relative = entry['path']
    original = (source / relative).read_text()
    current = (port / relative).read_text()
    if original != current:
        changed.append(relative)
        patch.extend(difflib.unified_diff(original.splitlines(keepends=True), current.splitlines(keepends=True), fromfile='a/' + relative, tofile='b/' + relative))
for relative in ('services/server/env/linux/mfw-dmabuf-fill.h', 'include/mfw-linux-compat.h', 'include/mfw-ddk-power.h', 'include/mfw-mfg-power-sequence.h', 'services/system/rgx_mtk/mfw-ddk-power.c'):
    changed.append(relative)
    patch.extend(difflib.unified_diff([], (port / relative).read_text().splitlines(keepends=True), fromfile='/dev/null', tofile='b/' + relative))
text = ''.join(patch)
(target / 'linux73-drm-initial.patch').write_text(text, newline='\n')
metadata = {key: manifest[key] for key in ('repository', 'commit', 'subtree')}
metadata.update(files_verified=590, verification='Git blob SHA1: all source files match pinned tree', changed_files=changed, patch_sha256=hashlib.sha256(text.encode()).hexdigest(), state='Native ARM64 kernel port; exact build/deployment/runtime evidence is in phone/hardware/gpu-native-v*-validation.json and docs/GPU_DDK19_PORT.md. This source provenance does not imply Mesa/rendering support.')
(target / 'source-provenance.json').write_text(json.dumps(metadata, indent=2) + '\n', newline='\n')
# Full manifest is public source metadata only, used for exact offline verification.
shutil.copyfile(base / 'out/ddk19-source-tree.json', target / 'source-tree.json')
for name in ('prepare_ddk19_port.py', 'compile_ddk19_port.sh', 'audit_ddk19_port.sh', 'export_ddk19_port.py', 'replay_ddk19_port.py', 'port_ddk19_dmabuf.py', 'port_ddk19_fences.py', 'test_ddk19_dmabuf_fill.c', 'test_ddk19_dmabuf.sh', 'test_ddk19_strings.c', 'port_ddk19_osfunc.py', 'port_ddk19_headers.py', 'port_ddk19_common.py', 'port_ddk19_memory.py', 'port_ddk19_misc.py', 'port_ddk19_arm64_cache.py', 'port_ddk19_optional.py', 'port_ddk19_fallthrough.py', 'port_ddk19_platform.py', 'port_ddk19_fence_cleanup.py', 'port_ddk19_stats.py', 'sync_ddk19_wsl.sh', 'build_ddk19_wsl.sh', 'link_ddk19_wsl.sh', 'check_ddk19_kernel_symbols.sh', 'summarize_ddk19_build.py', 'fetch_mesa_srv_reference.py', 'build_native_pvr_probe.sh', 'check_native_pvr_probe.sh'):
    shutil.copyfile(base / name, repo / 'archive/pc_work' / name)
print('Exported:', ', '.join(changed))
