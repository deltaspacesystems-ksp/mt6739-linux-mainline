import struct,sys
img=open(r"D:\y5_linux_work\out\stock_Image","rb").read()
B=0xffffff8008080000
def off(va): return va-B
def cstr(va):
    o=off(va); e=img.index(b"\0",o); return img[o:e].decode("ascii","replace")
def u32(va): return struct.unpack_from("<I",img,off(va))[0]
def u64(va): return struct.unpack_from("<Q",img,off(va))[0]
tab=int(sys.argv[1],16); n=int(sys.argv[2],16)
for i in range(n):
    p=tab+i*0x30
    id_=u32(p); name=cstr(u64(p+8)); par=u64(p+0x10); par=cstr(par) if par else ""
    regs=u64(p+0x18); sta,clr,st=u32(regs),u32(regs+4),u32(regs+8)
    bit=img[off(p+0x20)]
    print("%2d id=%d %-28s parent=%-22s sta=%#x clr=%#x set=%#x bit=%d"%(i,id_,name,par,sta,clr,st,bit))
