from pathlib import Path
import subprocess
import tempfile
base=Path('/mnt/d/y5_linux_work')
relative='drivers/gpu/drm/mfw-rogue-ddk19/kernel/drivers/staging/imgtec/pvr_drm.c'
patch=base/'export/mfw-kernel-7.3-v61-on-v60.patch'
with tempfile.TemporaryDirectory(prefix='mfw-v61-check-') as directory:
    target=Path(directory)/relative
    target.parent.mkdir(parents=True)
    target.write_bytes((base/'export/pvr_drm-v60.c').read_bytes())
    subprocess.run(['patch','--batch','-p1','-i',str(patch)],cwd=directory,check=True,stdout=subprocess.DEVNULL)
    assert target.read_bytes()==(Path('/home/y5/linux-7.3')/relative).read_bytes()
    assert target.read_bytes()==(base/'rogue-ddk19-port/kernel/drivers/staging/imgtec/pvr_drm.c').read_bytes()
print('PASS: v61 delta applies to v60 and exactly reproduces compiled DRM source')
