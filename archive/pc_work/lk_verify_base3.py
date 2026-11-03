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
base = 0x60027300
for name, off in targets.items():
    pred = base + off
    print(f"{name:45s} -> {'OK' if pred in wordset else 'brak'}  (addr {hex(pred)})")
