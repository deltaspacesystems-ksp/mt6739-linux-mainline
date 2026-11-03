"""Recreate compile-only overlay from verified public source, then build DRM object."""
import subprocess
from pathlib import Path
import shutil
base = Path('D:/y5_linux_work')
subprocess.run(['python', str(base / 'prepare_ddk19_port.py')], check=True)
shutil.copyfile('X:/y5linux/phone/kernel/gpu/ddk19/linux73-drm-initial.patch', base / 'out/ddk19-port.patch')
# Only these owned additions are absent from the original source tree.
for relative in ('services/server/env/linux/mfw-dmabuf-fill.h', 'include/mfw-linux-compat.h', 'include/mfw-ddk-power.h', 'include/mfw-mfg-power-sequence.h', 'services/system/rgx_mtk/mfw-ddk-power.c'):
    (base / 'rogue-ddk19-port' / relative).unlink(missing_ok=True)
subprocess.run(['wsl', '-d', 'archlinux', '--', 'patch', '--batch', '--forward', '-p1', '-d', '/mnt/d/y5_linux_work/rogue-ddk19-port', '-i', '/mnt/d/y5_linux_work/out/ddk19-port.patch'], check=True)
subprocess.run(['wsl', '-d', 'archlinux', '--', 'bash', '/mnt/d/y5_linux_work/sync_ddk19_wsl.sh'], check=True)
subprocess.run(['wsl', '-d', 'archlinux', '--', 'bash', '/mnt/d/y5_linux_work/build_ddk19_wsl.sh'], check=True)
subprocess.run(['wsl', '-d', 'archlinux', '--', 'bash', '/mnt/d/y5_linux_work/link_ddk19_wsl.sh'], check=True)
subprocess.run(['wsl', '-d', 'archlinux', '--', 'bash', '/mnt/d/y5_linux_work/check_ddk19_kernel_symbols.sh'], check=True)
assert not (base / 'out/ddk19-missing-kernel-symbols.txt').read_text().strip()
subprocess.run(['wsl', '-d', 'archlinux', '--', 'bash', '/mnt/d/y5_linux_work/test_ddk19_dmabuf.sh'], check=True)
shutil.copyfile('X:/y5linux/phone/kernel/gpu/ddk19/native_probe.c', base / 'native_pvr_probe.c')
subprocess.run(['wsl', '-d', 'archlinux', '--', 'bash', '/mnt/d/y5_linux_work/build_native_pvr_probe.sh'], check=True)
print('PASS: exact-source replay, full ARM64 archive/link, kernel symbol resolution and sanitizer tests')
