import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
print(y5put.put("/usr/local/bin/ko_syms.py", r'''
import struct, sys
def syms(path):
    d = open(path, "rb").read()
    is64 = d[4] == 2
    shoff, = struct.unpack_from("<Q" if is64 else "<I", d, 0x28 if is64 else 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, (0x3A if is64 else 0x2E))
    def sh(i):
        o = shoff + i * shentsize
        name, typ = struct.unpack_from("<II", d, o)
        if is64:
            off, size = struct.unpack_from("<QQ", d, o + 0x18)
            link, info = struct.unpack_from("<II", d, o + 0x28)
            entsize, = struct.unpack_from("<Q", d, o + 0x38)
        else:
            off, size = struct.unpack_from("<II", d, o + 0x10)
            link, info = struct.unpack_from("<II", d, o + 0x18)
            entsize, = struct.unpack_from("<I", d, o + 0x24)
        return name, typ, off, size, link, entsize
    strtab_off = sh(shstrndx)[2]
    def secname(nameoff): e = d.index(b"\0", strtab_off + nameoff); return d[strtab_off + nameoff:e].decode()
    symtab = strtab = None
    for i in range(shnum):
        name, typ, off, size, link, entsize = sh(i)
        n = secname(name)
        if n == ".symtab": symtab = (off, size, entsize, link)
        if n == ".strtab": strtab = off
    if not symtab: return []
    off, size, entsize, link = symtab
    n = size // entsize
    out = []
    for i in range(n):
        o = off + i * entsize
        if is64:
            nameoff, info, other, shndx = struct.unpack_from("<IBBH", d, o)
        else:
            nameoff, value, sz, info, other, shndx = struct.unpack_from("<IIIBBH", d, o)
        bind = info >> 4; typ = info & 0xF
        e = d.index(b"\0", strtab + nameoff); nm = d[strtab + nameoff:e].decode(errors="replace")
        if nm: out.append((nm, shndx, bind, typ))
    return out
if __name__ == "__main__":
    want = set(sys.argv[2:])
    for nm, shndx, bind, typ in syms(sys.argv[1]):
        if not want or nm in want:
            defined = shndx != 0
            print(f"{nm}\tdefined={defined}\tshndx={shndx}\tbind={bind}")
''', "755"))
print(y5sh.run(r"""
for f in wmt_chrdev_wifi wmt_drv wlan_drv_gen2 bt_drv gps_drv fmradio_drv; do
  echo "== $f =="
  python3 /usr/local/bin/ko_syms.py /ro/vendor/lib/modules/$f.ko wifi_reset_end wifi_reset_start register_set_p2p_mode_handler
done
""", timeout=40))
