import sys, re, os, struct
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
ks_path = r"D:\y5_linux_work\dump\kallsyms.txt"
if not os.path.exists(ks_path) or os.path.getsize(ks_path) < 1_000_000:
    out = y5sh.run("cat /proc/kallsyms", timeout=120)
    open(ks_path, "w").write(out)
syms = []
for l in open(ks_path):
    p = l.split()
    if len(p) >= 3 and re.fullmatch(r"[0-9a-f]{16}", p[0]): syms.append((int(p[0],16), p[2]))
syms.sort()
print("symbols:", len(syms))
img = open(r"D:\y5_linux_work\Image","rb").read()
BASE = 0xffffff8008080000
def func(name):
    i = next(k for k,(a,n) in enumerate(syms) if n == name)
    a = syms[i][0]; j = i+1
    while syms[j][0] == a: j += 1
    return a, syms[j][0], syms[j][1]
md = Cs(CS_ARCH_ARM64, CS_MODE_ARM); md.detail = False
def dis(name, maxn=400):
    a, b, nxt = func(name)
    print("\n=== %s  0x%x..0x%x (%d B) -> next: %s" % (name, a, b, b-a, nxt))
    code = img[a-BASE:b-BASE]
    for ins in md.disasm(code, a):
        print("%x: %-8s %s" % (ins.address, ins.mnemonic, ins.op_str)); maxn -= 1
        if maxn <= 0: break
dis("bfmr_ioctl")
