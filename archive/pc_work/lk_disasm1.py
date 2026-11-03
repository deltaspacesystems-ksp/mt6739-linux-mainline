from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB
d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
# region kodu tuz przed klastrem stringow orange/red/power (388000-393900), krok co 4B (ARM32)
start, end = 387800, 394300
seg = d[start:end]
md = Cs(CS_ARCH_ARM, CS_MODE_ARM)
lines = []
for i in md.disasm(seg, start):
    lines.append(i)
print("ARM32 zdekodowanych instrukcji:", len(lines), "/ oczekiwane", (end-start)//4)
# pokaz te z 'cmp' i pobliskie branche
for idx, i in enumerate(lines):
    if i.mnemonic.startswith("cmp") or i.mnemonic.startswith("b") and i.mnemonic not in ("bic","bfi","bfc"):
        print(f"{i.address:#x}: {i.mnemonic} {i.op_str}")
