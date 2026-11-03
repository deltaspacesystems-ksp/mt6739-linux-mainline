import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
[ -e /dev/stpwmt ] || mknod /dev/stpwmt c 190 0; chmod 666 /dev/stpwmt
rm -f /aroot/data/prop.log; dmesg -c >/dev/null
( chroot /aroot /system/bin/sh -c 'LD_PRELOAD=/data/propshim64.so exec /vendor/bin/wmt_launcher -p /vendor/firmware/' >/tmp/wmtl.out 2>&1 </dev/null & )
sleep 25
cat /tmp/wmtl.out | tail -n 10
sort /aroot/data/prop.log | uniq -c | head
dmesg | grep -i -E 'wmt|stp|patch|firmware|consys|power|wifi|conn|error|fail' | grep -v -E 'VcoreFS|charger' | head -n 60
""", timeout=70))
