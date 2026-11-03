import re
from capstone import Cs, CS_ARCH_ARM64, CS_MODE_ARM
syms=[]
for l in open(r"D:\y5_linux_work\dump\kallsyms.txt"):
    p=l.split()
    if len(p)>=3 and re.fullmatch(r"[0-9a-f]{16}",p[0]): syms.append((int(p[0],16),p[2]))
syms.sort(); name={a:n for a,n in syms}
img=open(r"D:\y5_linux_work\Image","rb").read(); BASE=0xffffff8008080000
md=Cs(CS_ARCH_ARM64,CS_MODE_ARM)
def dis_at(a, title):
    j=next(k for k,(x,nm) in enumerate(syms) if x>a); b=syms[j][0]
    print("\n===",title,hex(a),name.get(a,"?"),b-a,"B")
    for i in md.disasm(img[a-BASE:b-BASE],a):
        ex=""
        if i.mnemonic=="bl": ex="   ; "+str(name.get(int(i.op_str.strip('#'),16),"?"))
        print("%x: %-6s %s%s"%(i.address,i.mnemonic,i.op_str,ex))
dis_at(0xffffff80089160a8,"cmd 0x40044207 setter")
dis_at(0xffffff8008916140,"cmd 0x80044206 getter")
