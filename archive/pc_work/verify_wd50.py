"""Check the new boot payload against its inputs and the proven v49 ramdisk."""
import hashlib
import pathlib
import struct
import zlib

work = pathlib.Path(r"D:\y5_linux_work")
new = (work / "wd_v50.img").read_bytes()
old = (work / "wd_v49.img").read_bytes()

def payload(blob):
    assert blob[:8] == b"ANDROID!"
    ks, ka, rs, ra, ss, sa, ta, page = struct.unpack_from("<8I", blob, 8)
    assert page == 2048 and ss == 0
    ramoff = page + (ks + page - 1) // page * page
    return (ka, ra, ta), blob[page:page+ks], blob[ramoff:ramoff+rs]

addr, kernel, ramdisk = payload(new)
old_addr, _, old_ramdisk = payload(old)
assert addr == old_addr
assert ramdisk == old_ramdisk, "v49 initramfs must remain byte-for-byte identical"
assert new[64:576] == old[64:576], "boot command line changed"
dec = zlib.decompressobj(31)
image = dec.decompress(kernel) + dec.flush()
assert image == (work / "out/Image_wd24").read_bytes()
assert dec.unused_data == (work / "out/v12.dtb").read_bytes()
assert len(new) < 24 * 1024 * 1024
assert len(image) < 14 * 1024 * 1024
digest = hashlib.sha1()
for data in (kernel, ramdisk, b""):
    digest.update(data)
    digest.update(struct.pack("<I", len(data)))
assert new[576:596] == digest.digest()
print("PASS: kernel, DTB, boot SHA1, partition size, v49 addresses/cmdline/initramfs")
print("SHA256", hashlib.sha256(new).hexdigest())
