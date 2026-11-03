from pathlib import Path
root = Path('D:/y5_linux_work/rogue-ddk19-port')
p = root / 'Makefile'
s = p.read_text()
line = 'ccflags-y += -include $(RGX_TOP)/include/mfw-linux-compat.h\n'
if line not in s:
    s += '\n' + line
p.write_text(s, newline='\n')
p = root / 'services/server/env/linux/pvr_debug.c'
s = p.read_text().replace('static IMG_INT DebugStatusSet(', 'static ssize_t DebugStatusSet(').replace('static IMG_INT DebugLevelSet(', 'static ssize_t DebugLevelSet(')
s = s.replace('(PVRSRV_ENTRY_WRITE_FUNC *)DebugStatusSet', 'DebugStatusSet').replace('(PVRSRV_ENTRY_WRITE_FUNC *)DebugLevelSet', 'DebugLevelSet')
p.write_text(s, newline='\n')
p = root / 'services/server/env/linux/km_apphint.c'
s = p.read_text().replace('#define APPHINT_HELP_ARRAY_SIZE(a) (sizeof((a))/(sizeof((a[0]))))', '#define APPHINT_HELP_ARRAY_SIZE(a) __builtin_choose_expr(\\\n\t__builtin_types_compatible_p(typeof(a), void *), 0, \\\n\tsizeof(a) / (sizeof((a)[0]) + 0))')
p.write_text(s, newline='\n')
p = root / 'services/server/env/linux/pvr_bridge_k.c'
s = p.read_text().replace('#include <drm/drmP.h>', '#include <drm/drm_device.h>\n#include <drm/drm_file.h>\n#include <drm/drm_print.h>')
p.write_text(s, newline='\n')
p = root / 'services/server/devices/rgx/rgxfwutils.c'
s = p.read_text().replace('PVRSRV_ERROR eError = IMG_FALSE;', 'PVRSRV_ERROR eError = PVRSRV_OK;')
p.write_text(s, newline='\n')
p = root / 'include/img_defs.h'
s = p.read_text()
s = s.replace('\t#define __returns_nonnull     __attribute__((returns_nonnull))', '\t#ifndef __returns_nonnull\n\t#define __returns_nonnull     __attribute__((returns_nonnull))\n\t#endif')
s = s.replace('\t#define __returns_nonnull\n', '\t#ifndef __returns_nonnull\n\t#define __returns_nonnull\n\t#endif\n')
p.write_text(s, newline='\n')
