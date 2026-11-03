import struct, zlib
d = open(r"D:\huawei_y5_backup\logo.bin","rb").read()
magic, size, name = struct.unpack_from("<II32s", d, 0)
print("magic", hex(magic), "size", size, "name", name.rstrip(b'\0'))
for hdr_size in (512, 1024, 256):
    off = hdr_size
    try:
        nblocks, = struct.unpack_from("<I", d, off)
        if 0 < nblocks < 20:
            print(f"\n--- hdr_size={hdr_size}: nblocks={nblocks}")
            sizes = struct.unpack_from("<%dI" % nblocks, d, off+4)
            print("block sizes:", sizes)
            pos = off + 4 + 4*nblocks
            for i, bs in enumerate(sizes[:3]):
                blob = d[pos:pos+bs]
                try:
                    raw = zlib.decompress(blob)
                    print(f"  block {i}: compressed {bs} -> decompressed {len(raw)} (sqrt-ish check: {len(raw)/2})")
                except Exception as e:
                    print(f"  block {i}: zlib error {e}, first bytes {blob[:8].hex()}")
                pos += bs
    except Exception as e:
        print(hdr_size, "err", e)
