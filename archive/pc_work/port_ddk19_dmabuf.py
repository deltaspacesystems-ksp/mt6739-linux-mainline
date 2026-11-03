"""Port the reviewed dma-buf translation unit from pinned source; no hardware access."""
from pathlib import Path
base = Path('D:/y5_linux_work')
relative = 'services/server/env/linux/physmem_dmabuf.c'
s = (base / 'sources/rogue-ddk19' / relative).read_text()
s = s.replace('#include <linux/dma-buf.h>', '#include <linux/dma-buf.h>\n#include <linux/iosys-map.h>')
s = s.replace('struct dma_buf *psDmaBuf, struct device *psDev,', 'struct dma_buf *psDmaBuf,')
start = s.index('static void *PVRDmaBufOpsKMap(')
end = s.index('static int PVRDmaBufOpsMMap', start)
s = s[:start] + s[end:]
s = s.replace('\t.kmap_atomic   = PVRDmaBufOpsKMap,\n', '').replace('\t.kmap          = PVRDmaBufOpsKMap,\n', '')
# Import callbacks hold no dma_resv lock. Modern unlocked wrappers own it.
s = s.replace('dma_buf_map_attachment(', 'dma_buf_map_attachment_unlocked(')
s = s.replace('dma_buf_unmap_attachment(', 'dma_buf_unmap_attachment_unlocked(')
start = s.index('\tif (psPrivData->bPoisonOnFree)', s.index('static PVRSRV_ERROR PMRFinalizeDmaBuf'))
end = s.index('\nexit:', start)
s = s[:start] + '''\tif (psPrivData->bPoisonOnFree)
\t{
\t\tint err = mfw_fill_dmabuf(psDmaBuf, _FreePoison, _FreePoisonSize);
\t\tif (err)
\t\t\tPVR_DPF((PVR_DBG_ERROR, "%s: free poison failed (%d)", __func__, err));
\t}
''' + s[end:]
start = s.index('\tif (bZeroOnAlloc || bPoisonOnAlloc)')
end = s.index('\n\ttable = dma_buf_map_attachment_unlocked', start)
s = s[:start] + '''\tif (bZeroOnAlloc || bPoisonOnAlloc)
\t{
\t\tint err = mfw_fill_dmabuf(psDmaBuf,
\t\t\tbZeroOnAlloc ? NULL : _AllocPoison,
\t\t\tbZeroOnAlloc ? 0 : _AllocPoisonSize);
\t\tif (err)
\t\t{
\t\t\teError = PVRSRV_ERROR_PMR_NO_KERNEL_MAPPING;
\t\t\tgoto errFreePhysAddr;
\t\t}
\t}
''' + s[end:]
# Keep the original iosys_map alive until the matching release; no pointer casts.
start = s.index('static PVRSRV_ERROR\nPMRAcquireKernelMappingDataDmaBuf')
end = s.index('static PVRSRV_ERROR PMRMMapDmaBuf', start)
s = s[:start] + '''static PVRSRV_ERROR
PMRAcquireKernelMappingDataDmaBuf(PMR_IMPL_PRIVDATA pvPriv,
\t\tsize_t uiOffset, size_t uiSize, void **ppvKernelAddressOut,
\t\tIMG_HANDLE *phHandleOut, PMR_FLAGS_T ulFlags)
{
\tPMR_DMA_BUF_DATA *psPrivData = pvPriv;
\tstruct dma_buf *buf = psPrivData->psAttachment->dmabuf;
\tstruct iosys_map *map;
\tint err;

\tif (psPrivData->ui32PhysPageCount != psPrivData->ui32VirtPageCount ||
\t    uiOffset > buf->size || uiSize > buf->size - uiOffset)
\t\treturn PVRSRV_ERROR_PMR_NO_KERNEL_MAPPING;
\tmap = kzalloc(sizeof(*map), GFP_KERNEL);
\tif (!map)
\t\treturn PVRSRV_ERROR_OUT_OF_MEMORY;
\terr = dma_buf_begin_cpu_access(buf, DMA_BIDIRECTIONAL);
\tif (err)
\t\tgoto fail_free;
\terr = dma_buf_vmap_unlocked(buf, map);
\tif (err)
\t\tgoto fail_access;
\t/* PMR callers require an ordinary pointer, not an __iomem mapping. */
\tif (map->is_iomem || iosys_map_is_null(map))
\t{
\t\tdma_buf_vunmap_unlocked(buf, map);
\t\tgoto fail_access;
\t}
\t*ppvKernelAddressOut = (char *)map->vaddr + uiOffset;
\t*phHandleOut = map;
\treturn PVRSRV_OK;
fail_access:
\terr = dma_buf_end_cpu_access(buf, DMA_BIDIRECTIONAL);
\tif (err)
\t\tPVR_DPF((PVR_DBG_ERROR, "%s: end CPU access failed (%d)", __func__, err));
fail_free:
\tkfree(map);
\treturn PVRSRV_ERROR_PMR_NO_KERNEL_MAPPING;
}

static void PMRReleaseKernelMappingDataDmaBuf(PMR_IMPL_PRIVDATA pvPriv,
\t\tIMG_HANDLE hHandle)
{
\tPMR_DMA_BUF_DATA *psPrivData = pvPriv;
\tstruct dma_buf *buf = psPrivData->psAttachment->dmabuf;
\tstruct iosys_map *map = hHandle;
\tint err;

\tdma_buf_vunmap_unlocked(buf, map);
\terr = dma_buf_end_cpu_access(buf, DMA_BIDIRECTIONAL);
\tif (err)
\t\tPVR_DPF((PVR_DBG_ERROR, "%s: end CPU access failed (%d)", __func__, err));
\tkfree(map);
}

''' + s[end:]
s = s.replace('#include "kernel_compatibility.h"', '#include "kernel_compatibility.h"\n#include "mfw-dmabuf-fill.h"')
start = s.index('#if defined(PVR_ANDROID_ION_USE_SG_LENGTH)')
end = s.index('static const IMG_CHAR _AllocPoison', start)
s = s[:start] + '/* Modern attachment maps expose DMA segment lengths, including coalescing. */\n#define pvr_sg_length(sg) sg_dma_len(sg)\n\n' + s[end:]
needle = '\t/*\n\t * We do a two pass process:'
guard = '''\t/* Vendor secure-buffer zero-SG convention is unsupported on this port. */
\tif (!table->sgl || !table->nents) {
\t\teError = PVRSRV_ERROR_INVALID_PARAMS;
\t\tgoto errUnmap;
\t}
\tfor_each_sg(table->sgl, sg, table->nents, i) {
\t\tif (!sg_dma_len(sg) || !IS_ALIGNED(sg_dma_len(sg), PAGE_SIZE) ||
\t\t    !IS_ALIGNED(sg_dma_address(sg), PAGE_SIZE)) {
\t\t\teError = PVRSRV_ERROR_INVALID_PARAMS;
\t\t\tgoto errUnmap;
\t\t}
\t}

'''
assert needle in s
s = s.replace(needle, guard + needle, 1)
# PMR creation failure still owns the mapped attachment.
needle = '"%s: Failed to create PMR (%s)",'
pos = s.index(needle)
end = s.index('goto errFreePhysAddr;', pos)
s = s[:end] + s[end:].replace('goto errFreePhysAddr;', 'goto errUnmap;', 1)
start = s.index('static void _Poison(')
end = s.index('/*****************************************************************************', start)
s = s[:start] + s[end:]
(base / 'rogue-ddk19-port' / relative).write_text(s, newline='\n')
