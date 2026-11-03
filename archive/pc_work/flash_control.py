import sys, hashlib
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
local = r"D:\y5_linux_work\recovery_tools.img"
data = open(local, "rb").read(); n = len(data); want = hashlib.sha256(data).hexdigest()
push.push(local, "/tmp/ctl.img", port=2344)
cmd = r"""
DEV=""
for b in /sys/class/block/*; do grep -qx 'PARTNAME=recovery' $b/uevent 2>/dev/null && DEV=$b; done
MM=$(cat $DEV/dev); [ -e /dev/part_recovery ] || mknod /dev/part_recovery b ${MM%:*} ${MM#*:}
dd if=/tmp/ctl.img of=/dev/part_recovery bs=4096 2>&1 | tail -n 1
sync
echo readback_sha=$(dd if=/dev/part_recovery bs=4096 count=@B@ 2>/dev/null | head -c @N@ | sha256sum | cut -d' ' -f1)
""".replace("@B@", str((n + 4095)//4096)).replace("@N@", str(n))
out = y5sh.run(cmd, timeout=120); print(out.strip())
rb = [l for l in out.splitlines() if l.startswith("readback_sha=")]
print(">>> recovery = toolkit, ZGODNE:", bool(rb) and rb[0].split("=")[1].strip() == want)
