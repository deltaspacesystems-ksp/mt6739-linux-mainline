import sys, hashlib
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
src = r"D:\huawei_y5_backup\log.bin"
want = hashlib.sha256(open(src, "rb").read()).hexdigest()
print("lokalny sha256:", want, "rozmiar:", __import__("os").path.getsize(src))
print(push.push(src, "/tmp/log.bin", port=2340))
cmd = r"""
DEV=""
for b in /sys/class/block/*; do grep -qx 'PARTNAME=log' $b/uevent 2>/dev/null && DEV=$b; done
echo dev_sysfs=$DEV
MM=$(cat $DEV/dev); echo majmin=$MM
[ -e /dev/logpart ] || mknod /dev/logpart b ${MM%:*} ${MM#*:}
echo size_file=$(wc -c < /tmp/log.bin) sha_file=$(sha256sum /tmp/log.bin | cut -d' ' -f1)
dd if=/tmp/log.bin of=/dev/logpart bs=1048576 2>&1 | tail -n 1
sync
echo readback_sha=$(dd if=/dev/logpart bs=1048576 count=32 2>/dev/null | sha256sum | cut -d' ' -f1)
"""
out = y5sh.run(cmd, timeout=120)
print(out)
rb = [l for l in out.splitlines() if l.startswith("readback_sha=")]
print("ZGODNE:", bool(rb) and rb[0].split("=")[1].strip() == want)
