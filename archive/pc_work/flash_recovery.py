import sys, hashlib, os
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
src = r"D:\y5_linux_work\recovery_tools.img"
data = open(src, "rb").read(); n = len(data)
want = hashlib.sha256(data).hexdigest()
print("lokalny:", n, "B sha256", want)
print(push.push(src, "/tmp/rec.img", port=2341))
cmd = r"""
DEV=""
for b in /sys/class/block/*; do grep -qx 'PARTNAME=recovery' $b/uevent 2>/dev/null && DEV=$b; done
echo dev_sysfs=$DEV size_sectors=$(cat $DEV/size)
MM=$(cat $DEV/dev); [ -e /dev/recpart ] || mknod /dev/recpart b ${MM%:*} ${MM#*:}
echo file_sha=$(sha256sum /tmp/rec.img | cut -d' ' -f1)
dd if=/tmp/rec.img of=/dev/recpart bs=4096 2>&1 | tail -n 1
sync
echo readback_sha=$(dd if=/dev/recpart bs=4096 count=@@BLK@@ 2>/dev/null | head -c @@N@@ | sha256sum | cut -d' ' -f1)
""".replace("@@BLK@@", str((n + 4095)//4096)).replace("@@N@@", str(n))
out = y5sh.run(cmd, timeout=120); print(out)
rb = [l for l in out.splitlines() if l.startswith("readback_sha=")]
print("ZGODNE:", bool(rb) and rb[0].split("=")[1].strip() == want)
