"""Drop platform-only profiling hooks; preserve GPU execution and native fences."""
from pathlib import Path
import re
root = Path('D:/y5_linux_work/rogue-ddk19-port')
p = root / 'include/config_kernel_user_mt6739.h'
s = p.read_text()
for macro in ('ANDROID', 'SUPPORT_GPUTRACE_EVENTS', 'MTK_DEBUG_PROC_PRINT', 'MTK_USE_HW_APM', 'ENABLE_COMMON_DVFS'):
    s = re.sub(r'^#define ' + macro + r'(?:[^\n]*)\n', '', s, flags=re.M)
s = s.replace('"mt6739_android"', '"mt6739_microfw"')
p.write_text(s, newline='\n')
p = root / 'Makefile'
s = p.read_text()
for unit in ('services/server/env/linux/pvr_gputrace.o', 'services/server/env/linux/trace_events.o', 'services/system/rgx_mtk/mtk_pp.o'):
    s = s.replace('\t' + unit + ' \\\n', '')
p.write_text(s, newline='\n')
