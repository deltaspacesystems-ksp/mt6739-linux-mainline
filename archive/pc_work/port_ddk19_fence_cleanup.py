"""Scope fence teardown to a private workqueue and drain bounded retry chains."""
from pathlib import Path
root = Path('D:/y5_linux_work/rogue-ddk19-port')
p = root / 'kernel/drivers/staging/imgtec/pvr_fence.h'
s = p.read_text()
if 'int pvr_fence_global_init(void);' not in s:
    start = s.index('static inline void pvr_fence_cleanup(void)', s.index('struct pvr_fence_context'))
    end = s.index('#if defined(PVR_FENCE_DEBUG)', start)
    s = s[:start] + 'int pvr_fence_global_init(void);\nvoid pvr_fence_global_fini(void);\nvoid pvr_fence_cleanup(void);\n\n' + s[end:]
p.write_text(s, newline='\n')
p = root / 'kernel/drivers/staging/imgtec/pvr_fence.c'
s = p.read_text().replace('#include <linux/jiffies.h>', '#include <linux/jiffies.h>\n#include <linux/delay.h>')
point = s.index('#if defined(PVRSRV_USE_SYNC_CHECKPOINTS)')
s = s[:point] + '''static struct workqueue_struct *pvr_release_wq;
int pvr_fence_global_init(void)
{
\tpvr_release_wq = alloc_workqueue("mfw-pvr-release", WQ_UNBOUND | WQ_MEM_RECLAIM, 1);
\treturn pvr_release_wq ? 0 : -ENOMEM;
}
void pvr_fence_cleanup(void)
{
\t/* Retry work is queued with zero delay, so drain sees the entire chain. */
\tif (pvr_release_wq)
\t\tdrain_workqueue(pvr_release_wq);
}
void pvr_fence_global_fini(void)
{
\tif (pvr_release_wq) {
\t\tdestroy_workqueue(pvr_release_wq);
\t\tpvr_release_wq = NULL;
\t}
}

''' + s[point:]
old = 'schedule_delayed_work(&fctx->destroy_work,\n\t\t\t\t\t      msecs_to_jiffies(PVR_FENCE_CONTEXT_DESTROY_INITAL_WAIT_MS));'
if old not in s:
    # Whitespace varies in the vendor tree; match only this specific retry.
    import re
    s, n = re.subn(r'schedule_delayed_work\(&fctx->destroy_work,\s*destroy_delay_jiffies\);', 'msleep(jiffies_to_msecs(destroy_delay_jiffies));\n\t\t\tqueue_delayed_work(pvr_release_wq, &fctx->destroy_work, 0);', s)
    assert n == 1, n
else:
    s = s.replace(old, 'msleep(PVR_FENCE_CONTEXT_DESTROY_INITAL_WAIT_MS);\n\t\t\tqueue_delayed_work(pvr_release_wq, &fctx->destroy_work, 0);')
s = s.replace('schedule_delayed_work(&fctx->destroy_work, 0);', 'queue_delayed_work(pvr_release_wq, &fctx->destroy_work, 0);')
assert 'schedule_delayed_work' not in s
p.write_text(s, newline='\n')
p = root / 'kernel/drivers/staging/imgtec/pvr_platform_drv.c'
s = p.read_text().replace('#include "pvr_drv.h"', '#include "pvr_drv.h"\n#include "pvr_fence.h"')
s = s.replace('int ret = PVRSRVCommonDriverInit();\n\tif (ret)\n\t\treturn ret;', 'int ret = pvr_fence_global_init();\n\tif (ret)\n\t\treturn ret;\n\tret = PVRSRVCommonDriverInit();\n\tif (ret) {\n\t\tpvr_fence_global_fini();\n\t\treturn ret;\n\t}')
s = s.replace('if (ret)\n\t\tPVRSRVCommonDriverDeinit();', 'if (ret) {\n\t\tPVRSRVCommonDriverDeinit();\n\t\tpvr_fence_global_fini();\n\t}')
s = s.replace('/* Future explicit bringup only;', '#ifdef MODULE\n#error "MICROFW DDK bringup currently supports built-in only"\n#endif\n\n/* Future explicit bringup only;')
p.write_text(s, newline='\n')
