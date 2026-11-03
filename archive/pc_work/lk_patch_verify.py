d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
base = 0x60027300
table_runtime = 0x6002c3a8
file_off = table_runtime - base
print("plik offset tabeli:", hex(file_off), file_off)
table = d[file_off:file_off+4]
print("bajty tabeli (case0,1,2,3):", table.hex(' '))
assert list(table) == [0x16,0x12,0x0c,0x02], "NIE ZGADZA SIE - stop"
print("WERYFIKACJA OK: case0=0x16 case1=0x12 case2=0x0c case3=0x02")

# sprawdzmy tez docelowe adresy i ich sens (case1 -> 0x6002c11c to blok orange, case3 -> 0x6002c3ac to blok red)
for i,b in enumerate(table):
    tgt = table_runtime + 2*b
    print(f"case {i}: byte={hex(b)} -> target runtime {hex(tgt)} (file off {hex(tgt-base)})")
