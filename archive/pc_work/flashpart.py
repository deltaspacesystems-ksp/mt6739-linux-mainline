import sys, hashlib
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
def flash(local, partname, port=2350):
    data = open(local, "rb").read(); n = len(data); want = hashlib.sha256(data).hexdigest()
    print("%s -> %s (%d B, sha %s)" % (local.split(chr(92))[-1], partname, n, want[:16]))
    push.push(local, "/tmp/flash.img", port=port)
    cmd = r"""
DEV=""
for b in /sys/class/block/*; do grep -qx 'PARTNAME=@P@' $b/uevent 2>/dev/null && DEV=$b; done
MM=$(cat $DEV/dev); [ -e /dev/part_@P@ ] || mknod /dev/part_@P@ b ${MM%:*} ${MM#*:}
dd if=/tmp/flash.img of=/dev/part_@P@ bs=4096 2>&1 | tail -n 1
sync
echo readback_sha=$(dd if=/dev/part_@P@ bs=4096 count=@B@ 2>/dev/null | head -c @N@ | sha256sum | cut -d' ' -f1)
rm -f /tmp/flash.img
""".replace("@P@", partname).replace("@B@", str((n + 4095)//4096)).replace("@N@", str(n))
    out = y5sh.run(cmd, timeout=180); print(out.strip())
    rb = [l for l in out.splitlines() if l.startswith("readback_sha=")]
    ok = bool(rb) and rb[0].split("=")[1].strip() == want
    print(">>> ZGODNE:", ok); return ok
if __name__ == "__main__":
    sys.exit(0 if flash(sys.argv[1], sys.argv[2]) else 1)
