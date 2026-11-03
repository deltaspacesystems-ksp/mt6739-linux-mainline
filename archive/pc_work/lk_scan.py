import struct
d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
print("size", len(d))
print("first 16B:", d[:16])
import re
for pat in (b"Press", b"press", b"power", b"Power", b"verif", b"Verif", b"orange", b"continue"):
    idx = [m.start() for m in re.finditer(re.escape(pat), d)]
    if idx: print(pat, "at", idx[:10])
