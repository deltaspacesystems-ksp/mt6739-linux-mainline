from pathlib import Path
root = Path('D:/y5_linux_work/rogue-ddk19-port')
p = root / 'services/server/env/linux/osfunc_arm64.c'
s = p.read_text()
s = s.replace('#include <asm/cacheflush.h>', '#include <asm/cacheflush.h>\n#include <linux/dma-map-ops.h>')
s = s.replace('\tconst struct dma_map_ops *dma_ops;\n', '')
s = s.replace('\tdma_ops = get_dma_ops(psDevNode->psDevConfig->pvOSDevice);', '\t/* This interface receives CPU physical addresses, never DMA/IOVA handles. */\n\tif (WARN_ON(sCPUPhysEnd.uiAddr < sCPUPhysStart.uiAddr))\n\t\treturn;')
s = s.replace('dma_ops->sync_single_for_device(NULL,', 'arch_sync_dma_for_device(')
s = s.replace('dma_ops->sync_single_for_cpu(NULL,', 'arch_sync_dma_for_cpu(')
p.write_text(s, newline='\n')
