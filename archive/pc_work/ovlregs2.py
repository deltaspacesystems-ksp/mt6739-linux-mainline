import sys, re
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def rd(base, off, words):
    a = base + off
    out = y5sh.run("dd if=/dev/mem bs=4 skip=%d count=%d 2>/dev/null | od -An -v -tx4" % (a // 4, words), timeout=20)
    vals = re.findall(r"\b[0-9a-f]{8}\b", out)
    return vals
def dump(title, base, off, words):
    v = rd(base, off, words)
    print("\n##", title, hex(base + off), "(%d slow)" % len(v))
    for i in range(0, len(v), 4):
        print("  +0x%03x: %s" % (off + i * 4, " ".join(v[i:i+4])))
print(y5sh.run("ls -l /dev/mem; busybox --list | grep -E '^(od|hexdump|xxd)$' | tr '\\n' ' '", timeout=10))
dump("OVL0 reg 0x00-0x7f", 0x1400a000, 0x000, 32)
dump("OVL0 L0 addr area", 0x1400a000, 0xf40, 8)
dump("RDMA0 0x00-0x3f", 0x1400b000, 0x000, 16)
dump("RDMA0 0xf00", 0x1400b000, 0xf00, 8)
