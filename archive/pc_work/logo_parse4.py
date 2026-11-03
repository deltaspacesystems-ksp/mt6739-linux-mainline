import zlib
d = open(r"D:\huawei_y5_backup\logo.bin","rb").read()
hits = []
i = 0
while i < len(d)-1:
    if d[i] == 0x78 and d[i+1] in (0x9c, 0xda, 0x01, 0x5e):
        try:
            do = zlib.decompressobj()
            raw = do.decompress(d[i:i+2_000_000])
            consumed = len(d[i:i+2_000_000]) - len(do.unused_data)
            if len(raw) > 1000:
                hits.append((i, consumed, len(raw)))
        except Exception:
            pass
    i += 1
print("znalezionych dobrych blokow:", len(hits))
for off, cons, rawlen in hits[:50]:
    print(f"off={off:>8} consumed={cons:>8} raw={rawlen:>8}  raw/2={rawlen/2}")
