"""Measured v60 failure: DRM7.3 requires FOP_UNSIGNED_OFFSET before driver open."""
from pathlib import Path
import difflib
base=Path('/mnt/d/y5_linux_work')
relative='kernel/drivers/staging/imgtec/pvr_drm.c'
path=base/'rogue-ddk19-port'/relative
before=path.read_text()
assert '.fop_flags' not in before
after=before.replace('static const struct file_operations pvr_drm_fops = {', 'static const struct file_operations pvr_drm_fops = {\n\t.fop_flags              = FOP_UNSIGNED_OFFSET,')
assert after!=before
target='drivers/gpu/drm/mfw-rogue-ddk19/'+relative
kernel=Path('/home/y5/linux-7.3')/target
assert kernel.read_text()==before
(base/'export/pvr_drm-v60.c').write_text(before,newline='\n')
path.write_text(after,newline='\n')
kernel.write_text(after,newline='\n')
(Path('/home/y5/rogue-ddk19-port')/relative).write_text(after,newline='\n')
patch=''.join(difflib.unified_diff(before.splitlines(True),after.splitlines(True),fromfile='a/'+target,tofile='b/'+target))
(base/'export/mfw-kernel-7.3-v61-on-v60.patch').write_text(patch,newline='\n')
print('Applied sole v61 change: mandatory unsigned-offset DRM fops flag')
