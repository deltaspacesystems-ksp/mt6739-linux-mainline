import re, sys
from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
syms=[]
for l in open(r"D:\y5_linux_work\dump\kallsyms.txt"):
    p=l.split()
    if len(p)>=3 and re.fullmatch(r"[0-9a-f]{16}",p[0]): syms.append((int(p[0],16),p[2]))
syms.sort(); name={a:n for a,n in syms}
img=open(r"D:\y5_linux_work\Image","rb").read(); BASE=0xffffff8008080000
md=Cs(CS_ARCH_ARM64,CS_MODE_ARM)
def fn(n):
    a=next(x for x,nm in syms if nm==n); j=next(k for k,(x,nm) in enumerate(syms) if x>a); return a,syms[j][0]
def dis(n):
    a,b=fn(n); print("\n===",n,hex(a),(b-a),"B")
    for i in md.disasm(img[a-BASE:b-BASE],a):
        ex=""
        if i.mnemonic=="bl":
            t=int(i.op_str.strip("#"),16); ex="   ; "+str(name.get(t,"?"))
        print("%x: %-6s %s%s"%(i.address,i.mnemonic,i.op_str,ex))
for n in ("bfm_set_boot_timer_timeout_value","bfmr_resume_boot_timer","bfm_get_boot_timer_state"): dis(n)
# watek timera: symbol zawierajacy 'boot_timer' / 'hwboot' / 'bfm_boot_timer'
cands=[nm for a,nm in syms if re.search(r"boot_timer|hwboot_t|bfm_.*timer|boot_ats", nm)]
print("\ncandidates:", sorted(set(cands)))
