import re, struct
d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
targets = {}
for s in (b"boot state: orange", b"boot state: yellow", b"boot state: red", b"boot state: green",
          b"androidboot.verifiedbootstate=orange", b"androidboot.verifiedbootstate=red",
          b"Press Power key to continue.", b"Your device will boot in 5 seconds"):
    idx = d.find(s)
    if idx >= 0: targets[s] = idx
for s, off in targets.items(): print(off, s)

words = struct.unpack("<%dI" % (len(d)//4), d[:len(d)//4*4])
from collections import Counter
cnt = Counter()
for off in targets.values():
    cands = set()
    for w in words:
        base = w - off
        if 0 <= base <= 0x80000000:
            cands.add(base)
    for b in cands: cnt[b] += 1
best = cnt.most_common(10)
print("\nnajlepsze kandydatury bazy (ile stringow pasuje):")
for b, c in best: print(hex(b), c)
