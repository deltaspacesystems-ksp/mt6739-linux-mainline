import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /aroot/vendor
strings -a bin/ccci_fsd | grep -i -E '^/|ccci|boot|md_|ro\.|vendor\.|nvram|protect' | head -n 50
ps | grep -E 'ccci_fsd|rpcd|nvram' | grep -v grep
ls -l /dev | grep -i ccci | head; ls /aroot/dev | grep -i -c ccci
""", timeout=40))
