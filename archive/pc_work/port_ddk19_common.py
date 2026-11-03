"""Mainline-only configuration and mechanical shared Linux API updates."""
from pathlib import Path
import re
root = Path('D:/y5_linux_work/rogue-ddk19-port')
p = root / 'Makefile'
s = p.read_text()
if '-Wno-unused-but-set-variable' not in s:
    s = s.replace('\t-Werror', '\t-Wno-unused-but-set-variable \\\n\t-Wno-unused-const-variable \\\n\t-Wno-suggest-attribute=format \\\n\t-Werror')
p.write_text(s, newline='\n')
for p in root.rglob('*'):
    if p.suffix not in ('.c', '.h'):
        continue
    s = p.read_text()
    s = s.replace('access_ok(VERIFY_READ, ', 'access_ok(').replace('access_ok(VERIFY_WRITE, ', 'access_ok(')
    # Direct member replacement keeps existing lock acquisition mode unchanged.
    s = s.replace('->mmap_sem', '->mmap_lock')
    s = s.replace('ioremap_nocache(', 'ioremap(')
    if s != p.read_text():
        p.write_text(s, newline='\n')
# This driver imports standard dma-bufs; mainline has no Android ION allocator.
p = root / 'include/config_kernel_user_mt6739.h'
s = p.read_text()
for macro in ('SUPPORT_ION', 'PVR_ANDROID_ION_HEADER', 'PVR_ANDROID_ION_PRIV_HEADER', 'PVR_ANDROID_ION_USE_SG_LENGTH'):
    s = re.sub(r'^#define ' + macro + r'(?:\s[^\n]*)?\n', '', s, flags=re.M)
p.write_text(s, newline='\n')
p = root / 'Makefile'
s = p.read_text().replace('\tservices/system/rgx_mtk/ion_support.o \\\n', '')
p.write_text(s, newline='\n')
