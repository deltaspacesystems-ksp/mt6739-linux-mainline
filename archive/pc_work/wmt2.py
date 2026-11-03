import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /aroot/vendor/etc/init
for f in init.wmt_drv.rc init.wlan_drv.rc init_connectivity.rc; do echo "=== $f"; cat $f | grep -v '^\s*#' ; done | head -n 150
echo === devices; grep -i -E 'wmt|stp|gps|fm|bt|wifi|conn' /proc/devices
echo === kallsyms; grep -c -i wmt /proc/kallsyms
""", timeout=40))
