import struct, sys
d = open(sys.argv[1],'rb').read()
magic, total, off_struct, off_str, off_rsv, ver, lastcomp, bootcpu, sz_str, sz_struct = struct.unpack_from('>10I', d, 0)
strs = d[off_str:off_str+sz_str]
def cs(b, o):
    e = b.index(b'\0', o); return b[o:e].decode(errors='replace'), e+1
def fmt(name, v):
    if not v: return ""
    try:
        if v.endswith(b'\0') and all(32 <= c < 127 or c == 0 for c in v):
            return " = " + ", ".join('"%s"' % s.decode() for s in v[:-1].split(b'\0'))
    except Exception: pass
    if len(v) % 4 == 0:
        n = struct.unpack('>%dI' % (len(v)//4), v)
        return " = <" + " ".join("0x%x" % x for x in n[:32]) + (" ..." if len(n) > 32 else "") + ">"
    return " = [" + v.hex() + "]"
o = off_struct; depth = 0; lines = []
while True:
    tag, = struct.unpack_from('>I', d, o); o += 4
    if tag == 1:
        name, o = cs(d, o); o = (o + 3) & ~3
        lines.append("  "*depth + (name or "/") + " {"); depth += 1
    elif tag == 2:
        depth -= 1; lines.append("  "*depth + "};")
    elif tag == 3:
        ln, nameoff = struct.unpack_from('>II', d, o); o += 8
        v = d[o:o+ln]; o = (o + ln + 3) & ~3
        pn, _ = cs(strs, nameoff); lines.append("  "*depth + pn + fmt(pn, v) + ";")
    elif tag == 4: continue
    elif tag == 9: break
    else: break
open(sys.argv[2],'w').write("\n".join(lines))
print("dtb version", ver, "total", total, "| lines", len(lines))
