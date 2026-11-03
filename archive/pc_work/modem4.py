import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /aroot/vendor/etc/init
for f in init.cccimdinit.rc init.cccifsd.rc init.cccirpcd.rc rild.rc hw/init.modem.rc; do echo "=== $f"; cat $f; done | head -n 220
""", timeout=40))
