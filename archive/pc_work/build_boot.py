"""Buduje boot_alpine.bin: stockowy kernel Huawei 4.4.95 + wlasny initramfs (Alpine minirootfs + /init diagnostyczny)."""
import gzip, hashlib, io, os, stat, struct, tarfile, time, zlib

W = r"D:\y5_linux_work"
STOCK = r"D:\huawei_y5_backup\boot.bin"
ROOTFS = os.path.join(W, "alpine-minirootfs-3.24.2-aarch64.tar.gz")

INIT = r'''#!/bin/sh
# Y5 experiment init: zbiera diagnostyke, zapisuje do partycji `log`, probuje USB RNDIS + telnetd
export PATH=/bin:/sbin:/usr/bin:/usr/sbin
mount -t proc proc /proc 2>/dev/null
mount -t sysfs sysfs /sys 2>/dev/null
mount -t devtmpfs devtmpfs /dev 2>/dev/null
mkdir -p /tmp /run /dev/pts
mount -t tmpfs tmpfs /tmp 2>/dev/null
R=/tmp/report.txt
say() { echo "$@" >> $R; }
sect() { echo >> $R; echo "===== $1 =====" >> $R; }

# --- heartbeat: biale tlo na ekranie (dowod, ze nasz init ruszyl)
FB=/sys/class/graphics/fb0
if [ -d $FB ]; then
  VS=$(cat $FB/virtual_size 2>/dev/null); BPP=$(cat $FB/bits_per_pixel 2>/dev/null)
  W_=${VS%,*}; H_=${VS#*,}
  [ -e /dev/fb0 ] || mknod /dev/fb0 c 29 0 2>/dev/null
  if [ -n "$W_" ] && [ -n "$BPP" ]; then
    BYTES=$((W_ * H_ * BPP / 8))
    tr '\000' '\377' < /dev/zero 2>/dev/null | head -c $BYTES > /dev/fb0 2>/dev/null
  fi
fi

say "Y5 diag $(date 2>/dev/null)"
sect uname;        uname -a >> $R 2>&1
sect cmdline;      cat /proc/cmdline >> $R 2>&1
sect cpuinfo;      cat /proc/cpuinfo >> $R 2>&1
sect meminfo;      cat /proc/meminfo >> $R 2>&1
sect iomem;        cat /proc/iomem >> $R 2>&1
sect partitions;   cat /proc/partitions >> $R 2>&1
sect devices;      cat /proc/devices >> $R 2>&1
sect interrupts;   cat /proc/interrupts >> $R 2>&1
sect modules;      cat /proc/modules >> $R 2>&1
sect mounts;       cat /proc/mounts >> $R 2>&1
sect "fb0";        for f in name virtual_size bits_per_pixel stride modes mode; do echo "$f: $(cat $FB/$f 2>&1)" >> $R; done
sect "fb info";    ls -l /dev/fb* /sys/class/graphics 2>&1 >> $R
sect "power_supply"; for p in /sys/class/power_supply/*; do echo "-- $p" >> $R; cat $p/uevent >> $R 2>&1; done
sect "i2c devices"; for d in /sys/bus/i2c/devices/*; do echo "$d: $(cat $d/name 2>&1)" >> $R; done
sect "input";      cat /proc/bus/input/devices >> $R 2>&1
sect "udc";        ls -l /sys/class/udc >> $R 2>&1
sect "net";        ls /sys/class/net >> $R 2>&1
sect "block uevents"; for b in /sys/class/block/mmcblk0*; do echo "$b $(cat $b/dev 2>&1) $(grep PARTNAME $b/uevent 2>&1)" >> $R; done
sect "usb gadget (configfs)"
GOK=0
if mount -t configfs none /sys/kernel/config 2>>$R; then
  G=/sys/kernel/config/usb_gadget/g1
  mkdir $G 2>>$R && {
    echo 0x1d6b > $G/idVendor; echo 0x0104 > $G/idProduct
    mkdir -p $G/strings/0x409; echo y5 > $G/strings/0x409/product; echo y5lab > $G/strings/0x409/serialnumber
    mkdir -p $G/configs/c.1; mkdir -p $G/functions/rndis.usb0 2>>$R
    ln -s $G/functions/rndis.usb0 $G/configs/c.1/ 2>>$R
    U=$(ls /sys/class/udc 2>/dev/null | head -n1)
    if [ -n "$U" ]; then echo "$U" > $G/UDC 2>>$R && GOK=1; else say "brak UDC w /sys/class/udc"; fi
  }
fi
say "gadget_ok=$GOK"
if [ "$GOK" = 1 ]; then
  sleep 2
  IF=$(ls /sys/class/net | grep -E '^(rndis|usb)[0-9]' | head -n1)
  say "gadget iface: $IF"
  ifconfig $IF 172.16.42.1 netmask 255.255.255.0 up 2>>$R
  ifconfig $IF >> $R 2>&1
  (busybox --list | tr '\n' ' ') > /tmp/applets.txt
  say "applets: $(cat /tmp/applets.txt)"
  if busybox telnetd --help >/dev/null 2>&1; then
    busybox telnetd -l /bin/sh -b 172.16.42.1 2>>$R && say "telnetd na 172.16.42.1:23"
  else
    ( while true; do nc -l -p 23 -e /bin/sh; done ) >/dev/null 2>&1 &
    say "nc -e shell na 172.16.42.1:23 (jesli nc obsluguje -e)"
  fi
fi
mount -t debugfs none /sys/kernel/debug 2>>$R
sect "clk_summary";        cat /sys/kernel/debug/clk/clk_summary >> $R 2>&1
sect "regulator_summary";  cat /sys/kernel/debug/regulator/regulator_summary >> $R 2>&1
sect "pinctrl";            for f in /sys/kernel/debug/pinctrl/*/pinmux-pins /sys/kernel/debug/pinctrl/*/pinconf-pins; do echo "-- $f" >> $R; head -n 400 $f >> $R 2>&1; done
sect "gpio";               cat /sys/kernel/debug/gpio >> $R 2>&1
sect "devices_deferred";   cat /sys/kernel/debug/devices_deferred >> $R 2>&1
sect "dmesg";              dmesg >> $R 2>&1
sect "kallsyms count";     wc -l /proc/kallsyms >> $R 2>&1

# --- zapis raportu do partycji `log` (32 MB, backup: log.bin)
DEV=""
for b in /sys/class/block/*; do
  if grep -qx "PARTNAME=log" $b/uevent 2>/dev/null; then DEV=$b; fi
done
if [ -n "$DEV" ]; then
  MM=$(cat $DEV/dev); MAJ=${MM%:*}; MIN=${MM#*:}
  mknod /dev/logpart b $MAJ $MIN 2>/dev/null
  SZ=$(wc -c < $R)
  { printf 'Y5LOG1 %010d\n' $SZ; cat $R; } > /tmp/out.bin
  dd if=/tmp/out.bin of=/dev/logpart bs=512 conv=sync 2>/dev/null
  sync
  echo "zapisano $SZ B do $DEV ($MAJ:$MIN)" > /tmp/done.txt
else
  echo "nie znaleziono partycji log" > /tmp/done.txt
fi
# koniec: szare tlo = raport zapisany
if [ -n "$BYTES" ]; then tr '\000' '\125' < /dev/zero 2>/dev/null | head -c $BYTES > /dev/fb0 2>/dev/null; fi
exec sleep 100000000
'''

def build_cpio():
    out = io.BytesIO(); ino = [1]
    def entry(name, mode, data=b"", nlink=1, rdev=(0, 0), mtime=0):
        n = name.lstrip("./").encode() + b"\0" if name not in (".", "") else b".\0"
        hdr = b"070701" + b"".join(b"%08X" % v for v in (
            ino[0], mode, 0, 0, nlink, mtime, len(data), 0, 0, rdev[0], rdev[1], len(n), 0))
        ino[0] += 1
        out.write(hdr + n); out.write(b"\0" * ((4 - (110 + len(n)) % 4) % 4))
        out.write(data); out.write(b"\0" * ((4 - len(data) % 4) % 4))
    seen = set()
    with tarfile.open(ROOTFS, "r:gz") as t:
        for m in t.getmembers():
            name = m.name.lstrip("./")
            if not name or name in seen or name == "init":  # init podmieniamy wlasnym
                continue
            seen.add(name)
            if m.isdir():   entry(name, stat.S_IFDIR | (m.mode & 0o7777))
            elif m.issym(): entry(name, stat.S_IFLNK | 0o777, m.linkname.encode())
            elif m.isreg(): entry(name, stat.S_IFREG | (m.mode & 0o7777), t.extractfile(m).read())
            elif m.islnk():
                src = t.getmember(m.linkname); entry(name, stat.S_IFREG | (src.mode & 0o7777), t.extractfile(src).read())
    for dev, (maj, mini, mode) in {"dev/console": (5, 1, 0o600), "dev/null": (1, 3, 0o666), "dev/zero": (1, 5, 0o666),
                                   "dev/tty": (5, 0, 0o666), "dev/tty0": (4, 0, 0o600), "dev/mem": (1, 1, 0o600),
                                   "dev/fb0": (29, 0, 0o660), "dev/kmsg": (1, 11, 0o600)}.items():
        if dev not in seen: entry(dev, stat.S_IFCHR | mode, rdev=(maj, mini))
    entry("init", stat.S_IFREG | 0o755, INIT.encode())
    entry("TRAILER!!!", 0)
    return out.getvalue()

def main():
    cpio = build_cpio()
    rd = gzip.compress(cpio, compresslevel=9, mtime=0)
    open(os.path.join(W, "initramfs_alpine2.cpio.gz"), "wb").write(rd)
    print("cpio", len(cpio), "-> gz", len(rd))
    d = open(STOCK, "rb").read()
    ksz, kaddr, rsz, raddr, ssz, saddr, taddr, page = struct.unpack_from("<8I", d, 8)
    al = lambda n: (n + page - 1) // page * page
    kernel = d[page:page + ksz]
    roff = page + al(ksz); end = roff + al(rsz)
    sigtail = d[end:]; s0 = next(i for i, b in enumerate(sigtail) if b not in (0, 0xff))
    sig = sigtail[s0:].rstrip(b"\xff").rstrip(b"\0")
    hdr = bytearray(d[:page]); struct.pack_into("<I", hdr, 16, len(rd))
    sha = hashlib.sha1(); sha.update(kernel); sha.update(struct.pack("<I", ksz)); sha.update(rd)
    sha.update(struct.pack("<I", len(rd))); sha.update(struct.pack("<I", ssz))
    hdr[576:596] = sha.digest(); hdr[596:608] = b"\0" * 12
    pad = lambda b: b + b"\0" * (al(len(b)) - len(b))
    img = bytes(hdr) + pad(kernel) + pad(rd) + pad(sig)
    assert len(img) <= len(d), "obraz za duzy dla partycji boot"
    img += b"\xff" * (len(d) - len(img))
    open(os.path.join(W, "boot_alpine2.bin"), "wb").write(img)
    print("boot_alpine2.bin", len(img), "ramdisk", len(rd), "sha256", hashlib.sha256(img).hexdigest()[:16])
    # kontrola: ponowny odczyt
    k2, _, r2 = struct.unpack_from("<3I", img, 8)
    ro2 = page + al(k2)
    back = zlib.decompressobj(16 + zlib.MAX_WBITS).decompress(img[ro2:ro2 + r2])
    print("kontrola: ramdisk gunzip", len(back) == len(cpio), "| kernel bez zmian:", img[page:page + ksz] == kernel)

main()
