"""Przebudowuje partycje `logo` telefonu (format MediaTek: 512 B naglowka + tablica offsetow + strumienie zlib 720x1440 BGRA).
Zamienia wybrany obraz (domyslnie 0 = "HUAWEI powered by android") na czarny. Reszta (ikony ladowania itd.) zostaje.
python make_logo.py ORYGINAL.img WYNIK.img [indeks ...] [--raw INDEKS=PLIK.raw ...]   (raw = BGRA 720x1440, patrz render_logo.py)
Wgranie (BROM, robi wlasciciel): python mtk.py w logo WYNIK.img ; przywrocenie: ten sam polecenie z ORYGINAL.img."""
import struct, sys, zlib
src, dst = sys.argv[1], sys.argv[2]
rest = sys.argv[3:]
raws = {}
while "--raw" in rest:
    k = rest.index("--raw"); a, _, f = rest[k + 1].partition("="); raws[int(a)] = f; del rest[k:k + 2]
idxs = [int(x) for x in rest] or ([] if raws else [0])
b = open(src, "rb").read()
H = 512
magic, dsize = struct.unpack_from("<II", b, 0)
assert magic == 0x58881688, "to nie naglowek MTK"
data = b[H:H + dsize]
cnt, tot = struct.unpack_from("<II", data, 0)
offs = list(struct.unpack_from("<%dI" % cnt, data, 8))
ends = offs[1:] + [tot]
blobs = [data[offs[i]:ends[i]] for i in range(cnt)]
for i in idxs:
    raw = zlib.decompress(blobs[i])
    blobs[i] = zlib.compress(bytes(len(raw)), 9)
    print("obraz %d: %d B (rozpakowany %d) -> %d B (czarny)" % (i, ends[i] - offs[i], len(raw), len(blobs[i])))
for i, f in raws.items():
    old = zlib.decompress(blobs[i]); new = open(f, "rb").read()
    assert len(new) == len(old), "zly rozmiar raw: %d (oczekiwano %d)" % (len(new), len(old))
    blobs[i] = zlib.compress(new, 9)
    print("obraz %d: wlasny %s -> %d B" % (i, f, len(blobs[i])))
pos = 8 + 4 * cnt
new_offs = []
for bl in blobs:
    new_offs.append(pos); pos += len(bl)
newdata = struct.pack("<II", cnt, pos) + struct.pack("<%dI" % cnt, *new_offs) + b"".join(blobs)
assert len(newdata) == pos
hdr = bytearray(b[:H]); struct.pack_into("<I", hdr, 4, len(newdata))
pad = b"\0"   # tyl oryginalnej partycji to glownie zera
out = bytes(hdr) + newdata
out += pad * (len(b) - len(out))
assert len(out) == len(b)
open(dst, "wb").write(out)
# weryfikacja
d2 = out[H:H + len(newdata)]
c2, t2 = struct.unpack_from("<II", d2, 0); o2 = struct.unpack_from("<%dI" % c2, d2, 8); e2 = list(o2[1:]) + [t2]
for i in range(c2):
    zlib.decompress(d2[o2[i]:e2[i]])
print("OK: %d obrazow, dane %d B (bylo %d), plik %d B, pad=0x%02x" % (c2, len(newdata), dsize, len(out), pad[0]))
