import struct
d = open(r"D:\y5_linux_work\stock.dtb",'rb').read()
_, total, off_struct, off_str, *_ , sz_str, sz_struct = struct.unpack_from('>10I', d, 0)
strs = d[off_str:off_str+sz_str]
def cs(b,o): e=b.index(b'\0',o); return b[o:e].decode(errors='replace'), e+1
o=off_struct; path=[]
while True:
    tag,=struct.unpack_from('>I',d,o); o+=4
    if tag==1: n,o=cs(d,o); o=(o+3)&~3; path.append(n)
    elif tag==2: path.pop()
    elif tag==3:
        ln,no=struct.unpack_from('>II',d,o); o+=8; v=d[o:o+ln]; o=(o+ln+3)&~3
        pn,_=cs(strs,no); p="/".join(path)
        if (p.endswith("/memory") or p=="/memory" or p.endswith("memory")) and pn=="reg":
            w=struct.unpack('>%dI'%(len(v)//4),v)
            for i in range(0,len(w),4): print("RAM region: base 0x%x size 0x%x (%d MB)"%((w[i]<<32)|w[i+1],(w[i+2]<<32)|w[i+3],((w[i+2]<<32)|w[i+3])>>20))
        if pn=="reg" and "pstore" in p:
            w=struct.unpack('>%dI'%(len(v)//4),v); print("pstore reg: base 0x%x size 0x%x"%((w[0]<<32)|w[1],(w[2]<<32)|w[3]))
        if "chosen" in p and pn in ("linux,initrd-start","linux,initrd-end"): print(pn, v.hex())
    elif tag==9: break
