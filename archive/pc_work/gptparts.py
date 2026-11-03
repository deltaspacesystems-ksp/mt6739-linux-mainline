import struct
d = open(r"D:\huawei_y5_backup\gpt.bin",'rb').read()
print("gpt.bin size", len(d))
# gpt.bin = PMBR(512) + header(512) + entries ; znajdz 'EFI PART'
h = d.find(b'EFI PART'); print("EFI PART at", h)
sig, rev, hsz, crc, _, cur, bkp, first, last = struct.unpack_from('<8sIIIIQQQQ', d, h)
guid = d[h+56:h+72]; ent_lba, n, esz = struct.unpack_from('<QII', d, h+72)
print("entries:", n, "size", esz, "first_usable", first, "last_usable", last)
base = h + 512 if ent_lba == 2 else h + 512
parts = []
for i in range(n):
    e = d[base+i*esz: base+(i+1)*esz]
    if len(e) < 128 or e[:16] == b'\0'*16: continue
    s, t = struct.unpack_from('<QQ', e, 32)
    name = e[56:128].decode('utf-16le').rstrip('\0')
    parts.append((i+1, name, s, t))
for p in parts: print("p%-3d %-14s lba %-9d - %-9d size %6.1f MB" % (p[0], p[1], p[2], p[3], (p[3]-p[2]+1)*512/2**20))
