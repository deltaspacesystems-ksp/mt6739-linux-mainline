import hashlib
out = r"D:\y5_linux_work\out"
img = bytearray(open(out + r"\stock_Image", "rb").read())
pay = open(out + r"\payload.bin", "rb").read()
print("stockowy Image %d B; payload %d B" % (len(img), len(pay)))
print("naglowek (0..0x40) bez zmian, code0 =", bytes(img[0:4]).hex(), "(b 0x40)")
print("oryginalne 16 B pod 0x40:", bytes(img[0x40:0x50]).hex())
img[0x40:0x40 + len(pay)] = pay                     # podmiana wejscia (stext) na payload
open(out + r"\stub3_Image", "wb").write(img)
print("zapisano stub3_Image, sha256", hashlib.sha256(bytes(img)).hexdigest()[:16])
