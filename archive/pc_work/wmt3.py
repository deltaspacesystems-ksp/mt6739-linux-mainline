import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /proc/modules | head; echo ---; ls /aroot/vendor/lib/modules/ 2>&1 | head -n 40
grep -r 'ro.wlan.gen' /aroot/vendor/build.prop /aroot/vendor/default.prop 2>/dev/null
grep -i wmt /proc/kallsyms | head -n 8
ls /aroot/vendor/lib/modules/*wmt* /aroot/vendor/lib/modules/*wlan* 2>&1 | head
""", timeout=40))
