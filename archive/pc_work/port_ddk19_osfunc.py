"""Linux 7.3 OS adaptation in isolated DDK tree; no kernel config edits."""
from pathlib import Path
import re
base = Path('D:/y5_linux_work')
relative = 'services/server/env/linux/osfunc.c'
s = (base / 'sources/rogue-ddk19' / relative).read_text()
pattern = r'#if \(LINUX_VERSION_CODE >= KERNEL_VERSION\(4, 5, 0\)\)\n(.*?)#endif /\* \(LINUX_VERSION_CODE >= KERNEL_VERSION\(4, 5, 0\)\) \*/'
def raw_pfn(match):
    body = match.group(1)
    return body.split('#else\n', 1)[1] if '#else\n' in body else ''
s, count = re.subn(pattern, raw_pfn, s, flags=re.S)
assert count == 6, count
s = s.replace('down_write(&psMM->mmap_sem);', 'mmap_write_lock(psMM);').replace('up_write(&psMM->mmap_sem);', 'mmap_write_unlock(psMM);')
s = s.replace('psVMA->vm_flags |=  VM_MIXEDMAP;', 'vm_flags_set(psVMA, VM_MIXEDMAP);').replace('psVMA->vm_flags |= VM_MIXEDMAP;', 'vm_flags_set(psVMA, VM_MIXEDMAP);')
s = s.replace('err = vm_insert_mixed(psVMA, uiCPUVirtAddr, uiPFN);', 'vm_fault_t fault = vmf_insert_mixed(psVMA, uiCPUVirtAddr, uiPFN);\n\t\t\t\terr = (fault & VM_FAULT_ERROR) ? -EFAULT : 0;')
# find_vma must run while mmap lock is held and must contain the requested base.
s = s.replace('/* Find the Virtual Memory Area associated with the user base address */', 'mmap_write_lock(psMM);\n\t/* Find the VMA while protected against concurrent munmap. */')
s = s.replace('if (NULL == psVMA)\n\t{\n\t\teError = PVRSRV_ERROR_PMR_NO_CPU_MAP_FOUND;', 'if (!psVMA || sCpuVAddrBase < psVMA->vm_start || !psVMA->vm_file)\n\t{\n\t\tmmap_write_unlock(psMM);\n\t\teError = PVRSRV_ERROR_PMR_NO_CPU_MAP_FOUND;', 1)
s = s.replace('\t/* Acquire the memory sem */\n\tmmap_write_lock(psMM);', '\t/* mmap write lock remains held through all page-table changes. */')
s = s.replace('#include "ged_log.h"', '').replace('unsigned int _ged_log_owner;', '')
s = re.sub(r'^.*_ged_log_owner = ged_log_buf_alloc.*\n', '', s, flags=re.M)
s = s.replace('ged_log_buf_print2(_ged_log_owner, GED_LOG_ATTR_TIME, "%d / %d [+]", gsOwner->pid, gsOwner->tgid);', 'pr_debug("pvr bridge owner %d/%d acquired\\n", gsOwner->pid, gsOwner->tgid);')
s = s.replace('ged_log_buf_print2(_ged_log_owner, GED_LOG_ATTR_TIME, "%d / %d [-]", gsOwner->pid, gsOwner->tgid);', 'pr_debug("pvr bridge owner %d/%d released\\n", gsOwner->pid, gsOwner->tgid);')
s = s.replace('IMG_UINT32\tuiSize, uiPageCount=0;', '')
s = s.replace('\tuiPageCount = (1 << psMemHandle->ui32Order);\n\tuiSize = (uiPageCount * PAGE_SIZE);\n', '')
s = s.replace('IMG_UINT32 OSVSScanf(', 'IMG_UINT32 __scanf(2, 3) OSVSScanf(')
s = s.replace('return strncpy(pszDest, pszSrc, uSize);', 'size_t copied = strnlen(pszSrc, uSize);\n\tmemcpy(pszDest, pszSrc, copied);\n\tif (copied < uSize)\n\t\tmemset(pszDest + copied, 0, uSize - copied);\n\treturn pszDest;')
s = s.replace('struct timespec ts;\n\n\tgetrawmonotonic(&ts);\n\treturn (IMG_UINT64) ts.tv_sec * 1000000000 + ts.tv_nsec;', 'return ktime_get_raw_ns();')
s = s.replace('ioremap_nocache(', 'ioremap(')
s = s.replace('static void OSTimerCallbackWrapper(uintptr_t uData)', 'static void OSTimerCallbackWrapper(struct timer_list *timer)')
s = s.replace('TIMER_CALLBACK_DATA\t*psTimerCBData = (TIMER_CALLBACK_DATA*)uData;', 'TIMER_CALLBACK_DATA *psTimerCBData = container_of(timer, TIMER_CALLBACK_DATA, sTimer);')
s = s.replace('init_timer(&psTimerCBData->sTimer);', 'timer_setup(&psTimerCBData->sTimer, OSTimerCallbackWrapper, 0);')
s = s.replace('\tpsTimerCBData->sTimer.function = (void *)OSTimerCallbackWrapper;\n', '').replace('\tpsTimerCBData->sTimer.data = (uintptr_t)psTimerCBData;\n', '')
s = s.replace('del_timer_sync(', 'timer_delete_sync(')
(base / 'rogue-ddk19-port' / relative).write_text(s, newline='\n')
