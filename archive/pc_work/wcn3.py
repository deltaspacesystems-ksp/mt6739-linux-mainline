import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\shim\propshim64.so", "/aroot/data/propshim64.so", port=2395))
print(y5sh.run(r"""
cat /sys/class/wmtdetect/wmtdetect/dev
MM=$(cat /sys/class/wmtdetect/wmtdetect/dev); [ -e /dev/wmtdetect ] || mknod /dev/wmtdetect c ${MM%:*} ${MM#*:}; chmod 666 /dev/wmtdetect
rm -f /aroot/data/prop.log; dmesg -c >/dev/null
timeout 60 chroot /aroot /system/bin/sh -c 'LD_PRELOAD=/data/propshim64.so exec /vendor/bin/wmt_loader' 2>&1 </dev/null | tail -n 15; echo loader_rc=$?
sleep 1
dmesg | grep -i -E 'wmt|stp|detect|sdio|conn|power|combo|chip' | head -n 40
sort /aroot/data/prop.log | uniq -c | head -n 10
""", timeout=100))
