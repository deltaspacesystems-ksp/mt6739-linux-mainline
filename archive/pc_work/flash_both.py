import sys, hashlib
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
def flash(local, partname, remote, port):
    data = open(local, "rb").read(); n = len(data); want = hashlib.sha256(data).hexdigest()
    print("\n=== %s -> %s (%d B, sha %s)" % (local.split(chr(92))[-1], partname, n, want[:16]))
    push.push(local, remote, port=port)
    cmd = r"""
DEV=""
for b in /sys/class/block/*; do grep -qx 'PARTNAME=@P@' $b/uevent 2>/dev/null && DEV=$b; done
echo dev=$DEV sectors=$(cat $DEV/size)
MM=$(cat $DEV/dev); [ -e /dev/part_@P@ ] || mknod /dev/part_@P@ b ${MM%:*} ${MM#*:}
echo file_sha=$(sha256sum @R@ | cut -d' ' -f1)
dd if=@R@ of=/dev/part_@P@ bs=4096 2>&1 | tail -n 1
sync
echo readback_sha=$(dd if=/dev/part_@P@ bs=4096 count=@B@ 2>/dev/null | head -c @N@ | sha256sum | cut -d' ' -f1)
""".replace("@P@", partname).replace("@R@", remote).replace("@B@", str((n + 4095)//4096)).replace("@N@", str(n))
    out = y5sh.run(cmd, timeout=120); print(out.strip())
    rb = [l for l in out.splitlines() if l.startswith("readback_sha=")]
    ok = bool(rb) and rb[0].split("=")[1].strip() == want
    print(">>> ZGODNE:", ok); return ok
a = flash(r"D:\y5_linux_work\mainline_test.img", "recovery", "/tmp/mainline.img", 2342)
b = flash(r"D:\y5_linux_work\recovery_tools.img", "boot", "/tmp/toolkit.img", 2343) if a else False
print("\nWYNIK: recovery(mainline)=%s  boot(toolkit)=%s" % (a, b))
