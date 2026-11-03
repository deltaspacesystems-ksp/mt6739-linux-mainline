from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
start, end = 386000, 400000
seg = d[start:end]
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB); md.skipdata = True
ins = list(md.disasm(seg, start))
valid = [i for i in ins if i.mnemonic != '.byte']
print("valid:", len(valid), "/", len(ins))
# pokaz wszystkie cbz/cbnz/b*.w /bl kolo klastra 393700-397200 (gdzie sa stringi orange/red/Power)
near = [i for i in valid if 391000 <= i.address <= 397500]
for i in near:
    if i.mnemonic in ("b","bl","blx","cbz","cbnz") or i.mnemonic.startswith("b.") or i.mnemonic.startswith("it") or i.mnemonic.startswith("cmp"):
        print(f"{i.address:#x} ({i.address}): {i.mnemonic} {i.op_str}")
