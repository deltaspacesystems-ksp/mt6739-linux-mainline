from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, CS_OPT_SKIPDATA
d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
start, end = 387800, 394300
seg = d[start:end]
for name, mode in (("ARM32", CS_MODE_ARM), ("Thumb", CS_MODE_THUMB)):
    md = Cs(CS_ARCH_ARM, mode); md.skipdata = True
    lines = list(md.disasm(seg, start))
    valid = [i for i in lines if i.mnemonic != '.byte']
    print(name, "total:", len(lines), "valid:", len(valid))
    cmps = [i for i in valid if i.mnemonic.startswith("cmp")]
    print("  cmp count:", len(cmps))
    for i in cmps[:15]: print("   ", hex(i.address), i.mnemonic, i.op_str)
