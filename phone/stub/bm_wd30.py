"""Buduje mainline_test.img: kernel Linux 7.3 (Image.gz + DTB) + Alpine + prosty init pokazujacy tekst na ekranie."""
import gzip, hashlib, io, os, stat, struct, tarfile, zlib

W = r"D:\y5_linux_work"
STOCK = r"D:\huawei_y5_backup\boot.bin"
ROOTFS = os.path.join(W, "alpine-minirootfs-3.24.2-aarch64.tar.gz")
IMAGE = os.environ.get("Y5_IMAGE", os.path.join(W, "out", "Image"))
DTB = os.environ.get("Y5_DTB", os.path.join(W, "out", "mt6739-stockbase.dtb"))
OUT = os.environ.get("Y5_OUT", os.path.join(W, "mainline_test.img"))
KERNEL_ADDR = int(os.environ.get("Y5_KADDR", "0x40200000"), 16)

INIT = r'''#!/bin/sh
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
mount -t devtmpfs devtmpfs /dev 2>/dev/null
wdt x 0x10007000 0x22000000
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mount -t debugfs none /sys/kernel/debug 2>/dev/null
mkdir -p /tmp
while true; do
  {
    echo "=== Y5 mainline $(uname -r)  uptime $(cut -d' ' -f1 /proc/uptime) ==="
    echo "udc: $(ls /sys/class/udc 2>&1)  net: $(ls /sys/class/net 2>&1 | tr '\n' ' ')"
    echo "mmc: $(ls /sys/class/mmc_host 2>&1 | tr '\n' ' ')  blk: $(ls /sys/block 2>&1 | tr '\n' ' ')"
    echo "deferred: $(timeout 2 cat /sys/kernel/debug/devices_deferred 2>&1 | tr '\n' ' ' | cut -c1-200)"
    dmesg | tail -n 80
  } > /tmp/scr 2>&1
  y5txt /tmp/scr
  sleep 3
done
'''

def build_cpio():
    out = io.BytesIO(); ino = [1]
    def entry(name, mode, data=b"", nlink=1, rdev=(0, 0)):
        n = (name + "\0").encode() if name else b".\0"
        hdr = b"070701" + b"".join(b"%08X" % v for v in (ino[0], mode, 0, 0, nlink, 0, len(data), 0, 0, rdev[0], rdev[1], len(n), 0))
        ino[0] += 1
        out.write(hdr + n); out.write(b"\0" * ((4 - (110 + len(n)) % 4) % 4))
        out.write(data); out.write(b"\0" * ((4 - len(data) % 4) % 4))
    seen = set()
    with tarfile.open(ROOTFS, "r:gz") as t:
        for m in t.getmembers():
            name = m.name.lstrip("./")
            if not name or name in seen or name == "init": continue
            seen.add(name)
            if m.isdir():   entry(name, stat.S_IFDIR | (m.mode & 0o7777))
            elif m.issym(): entry(name, stat.S_IFLNK | 0o777, m.linkname.encode())
            elif m.isreg(): entry(name, stat.S_IFREG | (m.mode & 0o7777), t.extractfile(m).read())
            elif m.islnk():
                s = t.getmember(m.linkname); entry(name, stat.S_IFREG | (s.mode & 0o7777), t.extractfile(s).read())
    for dev, (maj, mini, mode) in {"dev/console": (5, 1, 0o600), "dev/null": (1, 3, 0o666), "dev/tty0": (4, 0, 0o600)}.items():
        if dev not in seen: entry(dev, stat.S_IFCHR | mode, rdev=(maj, mini))
    entry("sbin/wdt", stat.S_IFREG | 0o755, open(r"D:\\y5_linux_work\\out\\wdt", "rb").read())
    entry("sbin/y5cal", stat.S_IFREG | 0o755, open(r"D:\\y5_linux_work\\out\\y5cal", "rb").read())
    entry("sbin/y5txt", stat.S_IFREG | 0o755, open(r"D:\\y5_linux_work\\out\\y5txt", "rb").read())
    entry("init", stat.S_IFREG | 0o755, INIT.encode())
    entry("TRAILER!!!", 0)
    return out.getvalue()

rd = gzip.compress(build_cpio(), compresslevel=9, mtime=0)
if os.environ.get("Y5_RD_OUT"): open(os.environ["Y5_RD_OUT"], "wb").write(rd)
image = open(IMAGE, "rb").read(); dtb = open(DTB, "rb").read()
kern = gzip.compress(image, compresslevel=9, mtime=0) + dtb          # Image.gz-dtb
d = open(STOCK, "rb").read()
page = 2048
hdr = bytearray(d[:page])
struct.pack_into("<I", hdr, 8, len(kern))              # kernel_size
struct.pack_into("<I", hdr, 12, KERNEL_ADDR)           # kernel_addr
struct.pack_into("<I", hdr, 16, len(rd))               # ramdisk_size
al = lambda n: (n + page - 1) // page * page
sha = hashlib.sha1(); sha.update(kern); sha.update(struct.pack("<I", len(kern))); sha.update(rd)
sha.update(struct.pack("<I", len(rd))); sha.update(struct.pack("<I", 0))
hdr[576:596] = sha.digest(); hdr[596:608] = b"\0" * 12
pad = lambda b: b + b"\0" * (al(len(b)) - len(b))
ksz0, _, rsz0 = struct.unpack_from("<3I", d, 8)
roff0 = page + al(ksz0); end0 = roff0 + al(rsz0)
sigtail = d[end0:]; s0 = next(i for i, b in enumerate(sigtail) if b not in (0, 0xff))
sig = sigtail[s0:].rstrip(bytes([0xff])).rstrip(bytes([0]))
img = bytes(hdr) + pad(kern) + pad(rd) + pad(sig)
assert len(img) <= 48 * 1024 * 1024, "za duzy"
open(OUT, "wb").write(img)
print("Image %d B -> gz+dtb %d B | ramdisk gz %d B | obraz %d B (%.1f MB) | sha256 %s" % (
    len(image), len(kern), len(rd), len(img), len(img) / 2**20, hashlib.sha256(img).hexdigest()[:16]))
print("kernel_addr = 0x%x, tags 0x%x, ramdisk 0x%x" % struct.unpack_from("<3I", img, 12)[0:1] + struct.unpack_from("<I", img, 20)[0:1] + struct.unpack_from("<I", img, 20)[0:1] if False else "kernel_addr=0x%x" % struct.unpack_from("<I", img, 12)[0])
