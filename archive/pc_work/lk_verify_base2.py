import struct
d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
targets = {}
for s in (b"boot state: orange", b"boot state: yellow", b"boot state: red", b"boot state: green",
          b"androidboot.verifiedbootstate=orange", b"androidboot.verifiedbootstate=red",
          b"Press Power key to continue.", b"Your device will boot in 5 seconds",
          b"boot state: unknown"):
    idx = d.find(s)
    if idx >= 0: targets[s.decode()] = idx

words = struct.unpack("<%dI" % (len(d)//4), d[:len(d)//4*4])
wordset = set(words)

candidates = [0x447245c8,0x60027204,0x600272bc,0x600272c0,0x600272fc,0x60027300,0x60027304,0x6002730c,0x600271dc,0x600271f0]
for base in candidates:
    hits = sum(1 for off in targets.values() if (base+off) in wordset)
    print(hex(base), "trafien:", hits, "/", len(targets))

# lepsza metoda: dla kazdej pary (target_i, target_j) compute base z i-tego, sprawdz na WSZYSTKICH
best = (0, None)
for name_i, off_i in targets.items():
    for w in wordset:
        base = w - off_i
        if 0x30000000 <= base <= 0x50000000:
            hits = sum(1 for off in targets.values() if (base+off) in wordset)
            if hits > best[0]: best = (hits, base)
print("\nnajlepsza znaleziona baza w zakresie 0x30000000-0x50000000:", best)
