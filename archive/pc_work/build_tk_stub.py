"""Buduje toolkit_v5.img: stockowy kernel Huawei 4.4.95 + Alpine + init narzedziowy (RNDIS shell, bfmok, pstore)."""
import gzip, hashlib, io, os, stat, struct, tarfile, zlib

W = r"D:\y5_linux_work"
STOCK = r"D:\huawei_y5_backup\boot.bin"
ROOTFS = os.path.join(W, "alpine-minirootfs-3.24.2-aarch64.tar.gz")
OUT = os.path.join(W, "tk_stub.img")

EXTRA = {   # sciezka w ramdisku -> (plik lokalny, tryb)
    "usr/bin/bfmok":   (os.path.join(W, "bfmok"), 0o755),
    "usr/bin/fbsolid": (os.path.join(W, "fbsolid_arm64"), 0o755),
    "usr/bin/fbtest":  (os.path.join(W, "fbtest_arm64"), 0o755),
    "usr/local/bin/mkdevs":   (os.path.join(W, "guisrc", "mkdevs.sh"), 0o755),
    "usr/local/bin/chroot-up": (os.path.join(W, "guisrc", "chroot-up.sh"), 0o755),
    "usr/local/bin/wcn-up": (os.path.join(W, "guisrc", "wcn-up.sh"), 0o755),
    "usr/local/lib/propshim64.so": (os.path.join(W, "shim", "propshim64.so"), 0o755),
    "usr/local/bin/bootsplash": (os.path.join(W, "guisrc", "bootsplash.sh"), 0o755),
    "usr/local/bin/gpupresent": (os.path.join(W, "gpupresent"), 0o755),
    "usr/local/bin/gpu-present-up": (os.path.join(W, "guisrc", "gpu-present-up.sh"), 0o755),
}

INIT = r'''#!/bin/sh
# Y5 toolkit init (kernel stock 4.4.95): RNDIS + shell, wylaczony BFM, pstore z poprzedniego startu w /tmp/prev
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
mount -t proc proc /proc 2>/dev/null
mount -t sysfs sysfs /sys 2>/dev/null
mount -t devtmpfs devtmpfs /dev 2>/dev/null
mkdir -p /tmp /run /dev/pts
mount -t tmpfs tmpfs /tmp 2>/dev/null
mount -t debugfs none /sys/kernel/debug 2>/dev/null

# --- biale tlo = nasz init ruszyl
FB=/sys/class/graphics/fb0
fill() {  # fill <wartosc_oktalna>
  [ -d $FB ] || return
  VS=$(cat $FB/virtual_size 2>/dev/null); BPP=$(cat $FB/bits_per_pixel 2>/dev/null)
  W_=${VS%,*}; H_=${VS#*,}
  [ -e /dev/fb0 ] || mknod /dev/fb0 c 29 0 2>/dev/null
  [ -n "$W_" ] && [ -n "$BPP" ] && tr '\000' "$1" < /dev/zero 2>/dev/null | head -c $((W_ * H_ * BPP / 8)) > /dev/fb0 2>/dev/null
}
fill '\377'

# --- pstore: zachowaj log z poprzedniego startu (np. awaria mainline)
mkdir -p /tmp/prev
mount -t pstore pstore /sys/fs/pstore 2>/dev/null
cp /sys/fs/pstore/* /tmp/prev/ 2>/dev/null

# --- Huawei BFM: zglos "boot zakonczony" (zatrzymuje timery 10/30 min)
[ -e /dev/hw_bfm ] || mknod /dev/hw_bfm c 10 45 2>/dev/null
/usr/bin/bfmok >/dev/null 2>&1

# --- USB gadget RNDIS
mount -t configfs none /sys/kernel/config 2>/dev/null
G=/sys/kernel/config/usb_gadget/g1
mkdir $G 2>/dev/null
echo 0x1d6b > $G/idVendor; echo 0x0104 > $G/idProduct
mkdir -p $G/strings/0x409; echo y5-toolkit > $G/strings/0x409/product; echo Y5LAB > $G/strings/0x409/serialnumber
mkdir -p $G/configs/c.1 $G/functions/rndis.usb0
ln -s $G/functions/rndis.usb0 $G/configs/c.1/ 2>/dev/null
U=$(ls /sys/class/udc 2>/dev/null | head -n1)
[ -n "$U" ] && echo "$U" > $G/UDC
sleep 2
IF=$(ls /sys/class/net | grep -E '^(rndis|usb)[0-9]' | head -n1)
ifconfig ${IF:-rndis0} 172.16.42.1 netmask 255.255.255.0 up

# --- shell na 172.16.42.1:23 (wiele polaczen)
nc -lk -p 23 -e /bin/sh >/dev/null 2>&1 &

# --- trwaly Linux na userdata: sshd + sesja graficzna w tle (shell RNDIS zostaje niezalezny)
/usr/local/bin/mkdevs >/dev/null 2>&1
echo 200 > /sys/class/leds/lcd-backlight/brightness 2>/dev/null
(
  sleep 2
  /usr/local/bin/chroot-up >/tmp/chroot-up.log 2>&1
  if mountpoint -q /mnt/root; then
    chroot /mnt/root /usr/sbin/sshd -e 2>/tmp/sshd.log
    [ -e /mnt/root/root/.use_gpu ] && /usr/local/bin/gpu-present-up >/tmp/gpup.log 2>&1 &
    /usr/local/bin/bootsplash 2>/tmp/bootsplash.log
    chroot /mnt/root /usr/local/bin/y5-session >/tmp/session.log 2>&1
  fi
) &

echo "toolkit gotowy $(date)" > /tmp/ready
fill '\125'      # szary = gotowe
exec sleep 100000000
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
    for dev, (maj, mini, mode) in {"dev/console": (5, 1, 0o600), "dev/null": (1, 3, 0o666), "dev/zero": (1, 5, 0o666),
                                   "dev/tty": (5, 0, 0o666), "dev/tty0": (4, 0, 0o600), "dev/fb0": (29, 0, 0o660)}.items():
        if dev not in seen: entry(dev, stat.S_IFCHR | mode, rdev=(maj, mini))
    for path, (local, mode) in EXTRA.items():
        entry(path, stat.S_IFREG | mode, open(local, "rb").read())
    entry("init", stat.S_IFREG | 0o755, INIT.encode())
    entry("TRAILER!!!", 0)
    return out.getvalue()

cpio = build_cpio()
rd = gzip.compress(cpio, compresslevel=9, mtime=0)
d = open(STOCK, "rb").read()
ksz, kaddr, rsz, raddr, ssz, saddr, taddr, page = struct.unpack_from("<8I", d, 8)
al = lambda n: (n + page - 1) // page * page
kernel_stock = d[page:page + ksz]
_dec = zlib.decompressobj(31); _dec.decompress(kernel_stock); _dtb = _dec.unused_data
_stub = open(os.path.join(W, "stub", "stub.bin"), "rb").read()
_stub = _stub + bytes(1) * (21521424 - len(_stub))
_gz = bytearray(gzip.compress(_stub, compresslevel=9, mtime=0)); _gz[9] = 3
kernel = bytes(_gz) + _dtb
roff = page + al(ksz); end = roff + al(rsz)
sigtail = d[end:]; s0 = next(i for i, b in enumerate(sigtail) if b not in (0, 0xff))
sig = sigtail[s0:].rstrip(b"\xff").rstrip(b"\0")
hdr = bytearray(d[:page]); struct.pack_into("<I", hdr, 16, len(rd)); struct.pack_into("<I", hdr, 8, len(kernel)); ksz = len(kernel)
sha = hashlib.sha1(); sha.update(kernel); sha.update(struct.pack("<I", ksz)); sha.update(rd)
sha.update(struct.pack("<I", len(rd))); sha.update(struct.pack("<I", ssz))
hdr[576:596] = sha.digest(); hdr[596:608] = b"\0" * 12
pad = lambda b: b + b"\0" * (al(len(b)) - len(b))
img = bytes(hdr) + pad(kernel) + pad(rd) + pad(sig)
assert len(img) <= 48 * 1024 * 1024
open(OUT, "wb").write(img)
print("cpio", len(cpio), "-> gz", len(rd), "| obraz", len(img), "B | sha256", hashlib.sha256(img).hexdigest()[:16])
back = zlib.decompressobj(16 + zlib.MAX_WBITS).decompress(img[page + al(ksz):page + al(ksz) + len(rd)])
print("kontrola ramdisk:", len(back) == len(cpio), "| kernel bez zmian:", img[page:page + ksz] == kernel)
