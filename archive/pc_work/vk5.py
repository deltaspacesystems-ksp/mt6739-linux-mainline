import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\shim\propshim64.so", "/aroot/data/propshim64.so", port=2395))
print(y5sh.run(r"""
rm -f /aroot/data/prop.log
timeout 20 chroot /aroot /system/bin/sh -c 'LD_PRELOAD=/data/propshim64.so exec /data/vktest' 2>&1 </dev/null | tail -n 12
echo --- props; sort /aroot/data/prop.log | uniq -c | head
""", timeout=60))
