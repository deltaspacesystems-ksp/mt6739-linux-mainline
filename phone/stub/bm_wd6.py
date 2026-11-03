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
# --- ankieta: kazdy restart odpowiada na kolejne pytanie (licznik w RAM 0x444f0010), odpowiedz koduje czas do restartu
mount -t debugfs none /sys/kernel/debug 2>/dev/null
wdt 18
K=0
wdt r 0x444f0011; [ $? -eq 165 ] && { wdt r 0x444f0010; K=$?; }
NK=$((K+1)); [ $NK -ge 18 ] && NK=0
wdt w 0x444f0011 165; wdt w 0x444f0010 $NK
B=0
D=/sys/kernel/debug/devices_deferred
case $K in
1) [ -n "$(ls /sys/class/mmc_host 2>/dev/null)" ] && B=1;;
2) ls -d /sys/bus/platform/devices/11230000* >/dev/null 2>&1 && B=1;;
3) ls /sys/bus/platform/devices/11230000*/driver >/dev/null 2>&1 && B=1;;
4) ls -d /sys/bus/platform/devices/11200000* >/dev/null 2>&1 && B=1;;
5) ls /sys/bus/platform/devices/11200000*/driver >/dev/null 2>&1 && B=1;;
6) [ -e /sys/bus/platform/drivers/musb-mtk ] && B=1;;
7) [ -n "$(ls /sys/class/udc 2>/dev/null)" ] && B=1;;
8) ls /sys/bus/platform/drivers 2>/dev/null | grep -q tphy && B=1;;
9) [ -n "$(ls /sys/class/phy 2>/dev/null)" ] && B=1;;
10) [ -s $D ] && B=1;;
11) grep -q -i -E '11200000|usb|musb' $D 2>/dev/null && B=1;;
12) grep -q -i -E '11230000|mmc|msdc' $D 2>/dev/null && B=1;;
13) ls /sys/class/net 2>/dev/null | grep -q -E 'usb|rndis' && B=1;;
14) [ -e /sys/block/mmcblk0 ] && B=1;;
15) [ -e /sys/bus/platform/drivers/mtk-msdc ] && B=1;;
16) ls -d /sys/bus/platform/devices/11cc0000* >/dev/null 2>&1 && B=1;;
17) dmesg 2>/dev/null | grep -q -i musb && B=1;;
esac
if [ $K -eq 0 ]; then T=24; elif [ $B -eq 1 ]; then T=12; else T=6; fi
wdt $T
sleep 100
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
