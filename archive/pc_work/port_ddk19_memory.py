from pathlib import Path
import re
root = Path('D:/y5_linux_work/rogue-ddk19-port')
p = root / 'services/server/env/linux/pmr_os.c'
s = (Path('D:/y5_linux_work/sources/rogue-ddk19') / p.relative_to(root)).read_text()
pattern = r'#if \(LINUX_VERSION_CODE >= KERNEL_VERSION\(4, 5, 0\)\)\n(.*?)#endif[^\n]*'
s = re.sub(pattern, lambda m: m[1].split('#else\n', 1)[1] if '#else\n' in m[1] else '', s, flags=re.S)
s = re.sub(r'ps_vma->vm_flags \|= ([A-Z0-9_]+);', r'vm_flags_set(ps_vma, \1);', s)
s = s.replace('iStatus = vm_insert_mixed(ps_vma,', 'vm_fault_t fault = vmf_insert_mixed(ps_vma,')
s = re.sub(r'(vm_fault_t fault = vmf_insert_mixed\(.*?\);)', r'\1\n\t\t\tiStatus = (fault & VM_FAULT_ERROR) ? -EFAULT : 0;', s, flags=re.S)
p.write_text(s, newline='\n')
p = root / 'services/server/env/linux/physmem_osmem_linux.c'
s = (Path('D:/y5_linux_work/sources/rogue-ddk19') / p.relative_to(root)).read_text()
s = s.replace('static struct shrinker g_sShrinker;', 'static struct shrinker *g_psShrinker;')
s = s.replace('psShrinker == &g_sShrinker', 'psShrinker == g_psShrinker')
start = s.index('#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,12,0))\nstatic int\n_ShrinkPagePool')
end = s.index('/* Unregister the shrinker', start)
s = s[:start] + '''/* Register reclaim only after both caches are valid. Fail initialization
 * cleanly rather than creating a page pool which cannot be reclaimed. */
int LinuxInitPhysmem(void)
{
\tg_psLinuxPageArray = kmem_cache_create("pvr-pa", sizeof(PMR_OSPAGEARRAY_DATA), 0, 0, NULL);
\tif (!g_psLinuxPageArray)
\t\treturn -ENOMEM;
\tg_psLinuxPagePoolCache = kmem_cache_create("pvr-pp", sizeof(LinuxPagePoolEntry), 0, 0, NULL);
\tif (!g_psLinuxPagePoolCache)
\t\tgoto fail_array;
\tg_psShrinker = shrinker_alloc(0, "pvr-page-pool");
\tif (!g_psShrinker)
\t\tgoto fail_pool;
\tg_psShrinker->count_objects = _CountObjectsInPagePool;
\tg_psShrinker->scan_objects = _ScanObjectsInPagePool;
\tg_psShrinker->seeks = DEFAULT_SEEKS;
\tOSAtomicWrite(&g_iPoolCleanTasks, 0);
\tshrinker_register(g_psShrinker);
\treturn 0;
fail_pool:
\tkmem_cache_destroy(g_psLinuxPagePoolCache);
\tg_psLinuxPagePoolCache = NULL;
fail_array:
\tkmem_cache_destroy(g_psLinuxPageArray);
\tg_psLinuxPageArray = NULL;
\treturn -ENOMEM;
}

''' + s[end:]
# Unregister before taking the pool lock: shrinker callbacks also take it.
s = s.replace('\tIMG_UINT32 uiPagesFreed;\n\n\tif (OSAtomicRead', '\tIMG_UINT32 uiPagesFreed;\n\n\tshrinker_free(g_psShrinker);\n\tg_psShrinker = NULL;\n\tif (OSAtomicRead', 1)
s = s.replace('\tunregister_shrinker(&g_sShrinker);\n', '')
s = s.replace('vm_map_ram(ppsCleanArray, uiToClean, -1, pgprot)', 'vmap(ppsCleanArray, uiToClean, VM_MAP, pgprot)')
s = s.replace('vm_unmap_ram(pvAddr, uiToClean)', 'vunmap(pvAddr)')
# Use the existing explicit-protection vmap path for permanent kernel maps too.
s = s.replace('#if !defined(CONFIG_64BIT) || defined(PVRSRV_FORCE_SLOWER_VMAP_ON_64BIT_BUILDS)', '#if 1 /* Linux 7.3 vmap preserves requested cache protection. */')
s = s.replace('VM_READ | VM_WRITE,', 'VM_MAP,')
p.write_text(s, newline='\n')
p = root / 'services/server/env/linux/physmem_osmem_linux.h'
s = p.read_text().replace('void LinuxInitPhysmem(void);', 'int LinuxInitPhysmem(void);')
p.write_text(s, newline='\n')
p = root / 'services/server/env/linux/osfunc.c'
s = p.read_text().replace('\tLinuxInitPhysmem();', '''\tif (LinuxInitPhysmem()) {
#if defined(PVRSRV_USE_BRIDGE_LOCK)
\t\tOSFreeMem(g_pvBridgeBuffers);
\t\tg_pvBridgeBuffers = NULL;
#endif
\t\treturn PVRSRV_ERROR_OUT_OF_MEMORY;
\t}''')
p.write_text(s, newline='\n')
