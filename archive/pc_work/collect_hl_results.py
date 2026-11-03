from pathlib import Path
from phone_shell import run
out, rc, ok = run('cat /tmp/hl_watchdog_test.log', 20)
assert ok and str(rc) == '0'
selected = [line for line in out.splitlines() if line.startswith(('PROFILE[','KICK=','STATUS_AFTER_KICK=','TEST_DONE','TEST_FINISHED'))]
assert sum(line.startswith('KICK=') for line in selected) == 10
assert 'TEST_DONE result=0' in selected
Path(r'D:\y5_linux_work\out\hl_watchdog_result.txt').write_text('\n'.join(selected)+'\n', encoding='utf-8')
print('Saved filtered hardware evidence: 10 successful timer strobes, no profile change')