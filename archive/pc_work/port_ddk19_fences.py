"""Select the DDK's existing dma_fence backend, not Android staging sync."""
from pathlib import Path
base = Path('D:/y5_linux_work')
root = base / 'rogue-ddk19-port'
p = root / 'Makefile'
s = p.read_text().replace('services/server/env/linux/sync_native_server.o', 'services/server/env/linux/dma_fence_sync_native_server.o')
s = s.replace('kernel/drivers/staging/imgtec/pvr_sync2.o', 'kernel/drivers/staging/imgtec/pvr_sync_dma_fence.o \\\n\tkernel/drivers/staging/imgtec/pvr_counting_timeline.o \\\n\tkernel/drivers/staging/imgtec/pvr_sw_fence.o \\\n\tkernel/drivers/staging/imgtec/pvr_fence.o')
p.write_text(s, newline='\n')
p = root / 'kernel/drivers/staging/imgtec/pvr_sync_dma_fence.c'
s = p.read_text().replace('strlcpy(', 'strscpy(')
p.write_text(s, newline='\n')
p = root / 'include/config_kernel_user_mt6739.h'
s = p.read_text()
if '#define SUPPORT_DMA_FENCE' not in s:
    s += '\n#define SUPPORT_DMA_FENCE\n'
p.write_text(s, newline='\n')
p = root / 'kernel/drivers/staging/imgtec/pvr_sync_dma_fence.c'
s = p.read_text().replace('access_ok(VERIFY_READ, ', 'access_ok(').replace('access_ok(VERIFY_WRITE, ', 'access_ok(')
p.write_text(s, newline='\n')
for name in ('pvr_counting_timeline.c', 'pvr_sw_fence.c', 'pvr_fence.c'):
    p = root / 'kernel/drivers/staging/imgtec' / name
    s = p.read_text().replace('strlcpy(', 'strscpy(')
    s = s.replace('%llu#%u', '%llu#%llu')
    p.write_text(s, newline='\n')
