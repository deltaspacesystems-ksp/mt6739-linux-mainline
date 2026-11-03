import gzip, hashlib, struct, os
W = r"D:\y5_linux_work"
payload = open(os.path.join(W, "src", "probe", "probe.bin"), "rb").read()
dtb = open(os.path.join(W, "out", "mt6739-huawei-dra.dtb"), "rb").read()
kern = gzip.compress(payload, compresslevel=9, mtime=0) + dtb
# minimalny cpio (tylko TRAILER) jako ramdisk
def ent(name, mode, data=b""):
    n = (name + "\0").encode()
    h = b"070701" + b"".join(b"%08X" % v for v in (1, mode, 0, 0, 1, 0, len(data), 0, 0, 0, 0, len(n), 0))
    return h + n + b"\0" * ((4 - (110 + len(n)) % 4) % 4) + data + b"\0" * ((4 - len(data) % 4) % 4)
cpio = ent("init", 0o100755, b"#!/bin/sh\nexec sleep 1000000\n") + ent("TRAILER!!!", 0)
rd = gzip.compress(cpio, mtime=0)
d = open(r"D:\huawei_y5_backup\boot.bin", "rb").read(); page = 2048
hdr = bytearray(d[:page])
struct.pack_into("<I", hdr, 8, len(kern)); struct.pack_into("<I", hdr, 12, 0x40000000); struct.pack_into("<I", hdr, 16, len(rd))
sha = hashlib.sha1(); sha.update(kern); sha.update(struct.pack("<I", len(kern))); sha.update(rd)
sha.update(struct.pack("<I", len(rd))); sha.update(struct.pack("<I", 0))
hdr[576:596] = sha.digest(); hdr[596:608] = b"\0" * 12
al = lambda n: (n + page - 1) // page * page
pad = lambda b: b + b"\0" * (al(len(b)) - len(b))
img = bytes(hdr) + pad(kern) + pad(rd)
open(os.path.join(W, "probe_boot.img"), "wb").write(img)
print("probe_boot.img", len(img), "B | kernel gz+dtb", len(kern), "| ramdisk", len(rd), "| sha256", hashlib.sha256(img).hexdigest()[:16])
