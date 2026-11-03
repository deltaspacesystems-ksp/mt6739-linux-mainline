import sys, hashlib, os
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
src = r"D:\huawei_y5_backup\recovery.bin"
data = open(src, "rb").read(); n = len(data); want = hashlib.sha256(data).hexdigest()
print("recovery.bin:", n, "B sha256", want[:16])
print(push.push(src, "/tmp/recovery.bin", port=2345))
cmd = r"""
DEV=""
for b in /sys/class/block/*; do grep -qx 'PARTNAME=recovery' $b/uevent 2>/dev/null && DEV=$b; done
MM=$(cat $DEV/dev); [ -e /dev/part_recovery ] || mknod /dev/part_recovery b ${MM%:*} ${MM#*:}
echo file_sha=$(sha256sum /tmp/recovery.bin | cut -d' ' -f1)
dd if=/tmp/recovery.bin of=/dev/part_recovery bs=4096 2>&1 | tail -n 1
sync
echo readback_sha=$(dd if=/dev/part_recovery bs=4096 count=@B@ 2>/dev/null | sha256sum | cut -d' ' -f1)
rm -f /tmp/recovery.bin
""".replace("@B@", str(n // 4096))
out = y5sh.run(cmd, timeout=180); print(out.strip())
rb = [l for l in out.splitlines() if l.startswith("readback_sha=")]
print(">>> stockowy recovery przywrocony, ZGODNE:", bool(rb) and rb[0].split("=")[1].strip() == want)
