import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ps | grep -E 'mdinit' | grep -v grep
strings /aroot/vendor/bin/ccci_mdinit | grep -i -E '^/|ro\.|persist|vendor\.|md_|ril\.|ccci|boot' | head -n 60
echo --- ccci_dump; cat /proc/ccci_dump 2>/dev/null | grep -i -E 'md|state|boot|fail|err' | tail -n 25
""", timeout=40))
