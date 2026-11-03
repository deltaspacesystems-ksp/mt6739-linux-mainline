d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
# ARM64 funkcja prolog: stp x29,x30,[sp,#-N]! = 0xA9BZ7BFD (maska czesciowa), szukamy wzorca
import re
arm64_prolog = re.findall(rb"\xfd\x7b..\xa9", d)
print("ARM64 'stp x29,x30' wystapien:", len(arm64_prolog))
# ARM32 (A32) funkcja prolog: push {..., lr} = koncowka E92D w little-endian to 2D E9 .. ..
arm32_push = re.findall(rb"\x2d\xe9..", d)
print("ARM32 'push {..,lr}' (E92D) wystapien:", len(arm32_push))
# Thumb-2 push {.., lr} = B5xx (2 bajty)
thumb_push = re.findall(rb"..\xb5", d)
print("Thumb push wystapien (surowo, duzo false-positive):", len(thumb_push))
print("rozmiar pliku:", len(d))
