import struct,sys
for f in (r"D:\huawei_y5_backup\boot.bin", r"D:\y5_linux_work\wd_v3.img", r"D:\y5_linux_work\cp_v5.img"):
    d=open(f,"rb").read(2048)
    ks,ka,rs,ra,ss,sa,ta,ps=struct.unpack_from("<8I",d,8)
    print(f.split("\\")[-1], "ksz=%x kaddr=%x rdsz=%x rdaddr=%x 2ndsz=%x 2nd=%x tags=%x page=%d"%(ks,ka,rs,ra,ss,sa,ta,ps), "dtsz=%x"%struct.unpack_from("<I",d,40)[0])
