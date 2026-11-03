import struct, zlib
d = open(r"D:\huawei_y5_backup\logo.bin","rb").read()
# szukaj zlib magic (78 9c / 78 da / 78 01 / 78 5e) w pierwszych 2048 bajtach
for i in range(0, 2048):
    if d[i] == 0x78 and d[i+1] in (0x9c, 0xda, 0x01, 0x5e):
        print("mozliwy zlib header @", i, hex(d[i]), hex(d[i+1]))
# sprawdz tez okolice offsetu 512 i 1024 w hex
for off in (508, 512, 516, 1020, 1024, 1028):
    print(off, d[off:off+16].hex())
