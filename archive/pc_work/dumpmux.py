import struct,sys
img=open(r"D:\y5_linux_work\out\stock_Image","rb").read()
B=0xffffff8008080000
off=lambda va: va-B
def cstr(va):
    o=off(va); e=img.index(b"\0",o); return img[o:e].decode("ascii","replace")
u32=lambda va: struct.unpack_from("<I",img,off(va))[0]
u64=lambda va: struct.unpack_from("<Q",img,off(va))[0]
tab=int(sys.argv[1],16); n=int(sys.argv[2],16); S=int(sys.argv[3],16)
for i in range(n):
    p=tab+i*S
    try:
        nm=cstr(u64(p+8))
    except Exception: nm="?"
    pp=u64(p+0x10); par=cstr(u64(pp)) if pp else ""
    sb=lambda a: struct.unpack_from("<b",img,off(a))[0]
    print("%2d id=%d %-22s p0=%-14s mux=%#x set=%#x clr=%#x upd=%#x shift=%d width=%d gate=%d updsh=%d np=%d"%(i,u32(p),nm,par,u32(p+0x18),u32(p+0x1c),u32(p+0x20),u32(p+0x24),img[off(p+0x28)],img[off(p+0x29)],sb(p+0x2a),img[off(p+0x2b)],u32(p+0x2c)))
