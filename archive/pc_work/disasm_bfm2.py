import sys, re
from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
syms=[]
for l in open(r"D:\y5_linux_work\dump\kallsyms.txt"):
    p=l.split()
    if len(p)>=3 and re.fullmatch(r"[0-9a-f]{16}",p[0]): syms.append((int(p[0],16),p[2]))
syms.sort(); name={a:n for a,n in syms}
img=open(r"D:\y5_linux_work\Image","rb").read(); BASE=0xffffff8008080000
md=Cs(CS_ARCH_ARM64,CS_MODE_ARM)
def sym_for(a):
    best=None
    for s,n in syms:
        if s<=a: best=(s,n)
        else: break
    return best
def dis(start,end,title):
    print("\n===",title,hex(start),"..",hex(end))
    for i in md.disasm(img[start-BASE:end-BASE],start):
        extra=""
        m=re.match(r"#(0x[0-9a-f]+)$",i.op_str) if i.mnemonic=="bl" else None
        if m:
            t=int(m.group(1),16); extra="   ; "+(name.get(t) or "?")
        print("%x: %-6s %s%s"%(i.address,i.mnemonic,i.op_str,extra))
dis(0xffffff8008916cf8,0xffffff8008916d5c,"cf8..d5c (male funkcje przed bfmr_ioctl)")
for n in ("bfmr_disable_timer","bfm_suspend_boot_timer","bfm_init_boot_timer","bfm_resume_boot_timer"):
    a=[x for x,nm in syms if nm==n]
    if a:
        j=next(k for k,(x,nm) in enumerate(syms) if x>a[0]); dis(a[0],syms[j][0],n)
