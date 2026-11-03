import struct, zlib
d = open(r"D:\huawei_y5_backup\logo.bin","rb").read()
nblocks, total = struct.unpack_from("<II", d, 512)
print("nblocks", nblocks, "total", total)
sizes = struct.unpack_from("<%dI" % nblocks, d, 520)
print("sizes[:6]", sizes[:6], "... sum=", sum(sizes))
pos = 520 + 4*nblocks
print("pos startu blokow:", pos)
frames = []
for i, bs in enumerate(sizes):
    blob = d[pos:pos+bs]
    raw = zlib.decompress(blob)
    frames.append(raw)
    if i < 3 or i == nblocks-1:
        print(f"frame {i}: compressed {bs} -> raw {len(raw)} B  (720*1440*2={720*1440*2})")
    pos += bs
print("wszystkie ramki zdekodowane OK, liczba:", len(frames))
import os
os.makedirs(r"D:\y5_linux_work\logo_frames", exist_ok=True)
open(r"D:\y5_linux_work\logo_frames\frame0.raw","wb").write(frames[0])
