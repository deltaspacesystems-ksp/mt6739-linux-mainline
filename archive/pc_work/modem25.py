import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
sort /aroot/data/prop.log | uniq -c | sort -rn | head -n 30
for p in $(pidof ccci_fsd ccci_rpcd); do echo "== $p"; cat /proc/$p/wchan; echo; ls -l /proc/$p/fd 2>&1 | head -n 12; done
""", timeout=40))
