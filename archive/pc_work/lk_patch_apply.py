import hashlib
d = bytearray(open(r"D:\huawei_y5_backup\lk.bin","rb").read())
off = 0x50a8 + 3   # bajt case3 w tablicy TBB
assert d[off] == 0x02, "bajt juz zmieniony lub zly offset"
d[off] = 0x12
out = r"D:\y5_linux_work\lk_patched.bin"
open(out, "wb").write(d)
print("oryginal sha256:", hashlib.sha256(open(r'D:\huawei_y5_backup\lk.bin','rb').read()).hexdigest()[:16])
print("patched  sha256:", hashlib.sha256(bytes(d)).hexdigest()[:16])
print("rozmiar oryginal:", len(open(r'D:\huawei_y5_backup\lk.bin','rb').read()), "| patched:", len(d))
print(f"zmieniony bajt @ offset {hex(off)}: 0x02 -> 0x12")
