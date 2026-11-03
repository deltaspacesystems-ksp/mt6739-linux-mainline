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
mount -t proc proc /proc
mount -t sysfs sysfs /sys
mount -t devtmpfs devtmpfs /dev 2>/dev/null
mkdir -p /tmp; mount -t tmpfs tmpfs /tmp
exec >/dev/console 2>&1 </dev/console
echo
echo "=================================================="
echo " Huawei Y5 2018 (MT6739) - mainline Linux $(uname -r)"
echo "=================================================="
echo "cmdline: $(cat /proc/cmdline)"
echo
grep -E 'model name|^Hardware|CPU part' /proc/cpuinfo | sort | uniq -c
grep -E 'MemTotal|MemFree' /proc/meminfo
echo
echo "framebuffer: $(cat /sys/class/graphics/fb0/name 2>&1) $(cat /sys/class/graphics/fb0/virtual_size 2>&1)"
mount -t pstore pstore /sys/fs/pstore 2>/dev/null; echo "pstore: $(ls /sys/fs/pstore 2>&1 | tr '\n' ' ')"
echo
echo "dmesg (ostatnie 25 linii):"; dmesg | tail -n 25
echo
echo "--- USB gadget (RNDIS) + shell na 172.16.42.1:23"
mount -t configfs none /sys/kernel/config 2>/dev/null
G=/sys/kernel/config/usb_gadget/g1
mkdir $G 2>/dev/null
echo 0x1d6b > $G/idVendor; echo 0x0104 > $G/idProduct
mkdir -p $G/strings/0x409; echo y5-mainline > $G/strings/0x409/product
mkdir -p $G/configs/c.1 $G/functions/rndis.usb0
ln -s $G/functions/rndis.usb0 $G/configs/c.1/ 2>/dev/null
U=$(ls /sys/class/udc 2>/dev/null | head -n1); echo "udc: ${U:-brak}"
[ -n "$U" ] && echo "$U" > $G/UDC
sleep 2
IF=$(ls /sys/class/net | grep -E '^(rndis|usb)[0-9]' | head -n1)
[ -n "$IF" ] && ifconfig $IF 172.16.42.1 netmask 255.255.255.0 up && (nc -lk -p 23 -e /bin/sh >/dev/null 2>&1 &)
echo "net: $(ls /sys/class/net | tr '\n' ' ')"
echo "mmc: $(ls /sys/class/mmc_host 2>&1 | tr '\n' ' ') blk: $(ls /sys/block 2>&1 | tr '\n' ' ')"
echo "bt: $(ls /sys/class/bluetooth 2>&1 | tr '\n' ' ')"
# --- diagnostyka (initcalle skonczone): log do expdb + sygnal restartem przez /dev/mem
wdt 31
L=/tmp/y5log.txt
mount -t debugfs none /sys/kernel/debug 2>/dev/null
{
 echo "Y5LOG v2 uptime $(cut -d' ' -f1 /proc/uptime)"
 echo "== dmesg"; dmesg
 echo "== udc/net"; ls /sys/class/udc /sys/class/net 2>&1
 echo "== deferred"; cat /sys/kernel/debug/devices_deferred 2>&1
 echo "== usb drivers"; ls /sys/bus/platform/drivers 2>&1 | grep -i -E 'musb|tphy|usb|mtk'
 echo "== usb devices"; ls /sys/bus/platform/devices 2>&1 | grep -i -E '11200000|11cc0000|usb|phy'
 echo "== iomem"; cat /proc/iomem 2>&1 | head -60
 echo "== clk"; cat /sys/kernel/debug/clk/clk_summary 2>&1 | head -40
 echo "== mmc/blk"; ls /sys/class/mmc_host /sys/block 2>&1
} > $L 2>&1
P=""
for d in /sys/block/mmcblk0/mmcblk0p*; do
  grep -q 'PARTNAME=expdb' $d/uevent 2>/dev/null && P=/dev/$(basename $d)
done
echo "expdb: ${P:-brak}" >> $L
if [ -n "$P" ]; then
  [ -b "$P" ] || { mknod $P b $(cut -d: -f1 /sys/block/mmcblk0/$(basename $P)/dev) $(cut -d: -f2 /sys/block/mmcblk0/$(basename $P)/dev); }
  SZ=$(wc -c < $L)
  { printf 'Y5LOG%08d\n' $SZ; cat $L; } | dd of=$P bs=4096 conv=fsync 2>/dev/null && echo done > /tmp/y5done
fi
M=0
[ -n "$(ls /sys/class/mmc_host 2>/dev/null)" ] && M=$((M+1))
[ -f /tmp/y5done ] && M=$((M+2))
[ -n "$(ls /sys/class/udc 2>/dev/null)" ] && M=$((M+4))
wdt $((16+2*M))
n=0
while true; do
  n=$((n+1)); echo "[init] uptime $(cut -d' ' -f1 /proc/uptime) s  (tick $n)"
  sleep 5
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
