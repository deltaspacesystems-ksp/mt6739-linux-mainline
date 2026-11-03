"""Preserve shared user headers while choosing kernel equivalents in kernel builds."""
from pathlib import Path
import re
root = Path('D:/y5_linux_work/rogue-ddk19-port')
for p in root.rglob('*'):
    if p.suffix not in ('.c', '.h'):
        continue
    s = p.read_text()
    for old, new in [('stddef.h', 'linux/stddef.h'), ('stdarg.h', 'linux/stdarg.h'), ('stdbool.h', 'linux/types.h')]:
        # Skip files already containing an explicit kernel/user include choice.
        if f'#include <{new}>' in s:
            continue
        s = re.sub(r'^([ \t]*)#include <' + re.escape(old) + r'>([^\n]*)$', lambda m: '#ifdef __KERNEL__\n#include <' + new + '>\n#else\n' + m[0] + '\n#endif', s, flags=re.M)
    if p.name == 'pvr_uaccess.h':
        s = s.replace('access_ok(VERIFY_READ, ', 'access_ok(').replace('access_ok(VERIFY_WRITE, ', 'access_ok(')
    if s != p.read_text():
        p.write_text(s, newline='\n')
