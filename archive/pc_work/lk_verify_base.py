import struct
d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
targets = {}
for s in (b"boot state: orange", b"boot state: yellow", b"boot state: red", b"boot state: green",
          b"androidboot.verifiedbootstate=orange", b"androidboot.verifiedbootstate=red",
          b"Press Power key to continue.", b"Your device will boot in 5 seconds",
          b"boot state: unknown", b"Keep pressed power button to boot"):
    idx = d.find(s)
    if idx >= 0: targets[s.decode()] = idx

base = 0x447245c8 - 393828  # 393828 to offset "boot state: orange"
print("testowana baza:", hex(base))
words = struct.unpack("<%dI" % (len(d)//4), d[:len(d)//4*4])
wordset = set(words)
hits = 0
for name, off in targets.items():
    pred = base + off
    ok = pred in wordset
    print(f"  {name:45s} off={off:>7} pred_addr={hex(pred)} -> literal present: {ok}")
    hits += ok
print("trafien:", hits, "/", len(targets))
