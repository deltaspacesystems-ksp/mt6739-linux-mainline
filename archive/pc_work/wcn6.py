import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\shim\propshim64.so", "/aroot/data/propshim64.so", port=2395))
print(y5sh.run(r"""
pkill wmt_launcher; sleep 1
rm -f /aroot/data/prop.log; dmesg -c >/dev/null
( chroot /aroot /system/bin/sh -c 'LD_PRELOAD=/data/propshim64.so exec /vendor/bin/wmt_launcher -p /vendor/firmware/' >/tmp/wmtl.out 2>&1 </dev/null & )
sleep 25
tail -n 8 /tmp/wmtl.out
dmesg | grep -E 'WMT|STP|HIF|PATCH|CONSYS|WIFI|BT|GPS' | head -n 70
""", timeout=80))
