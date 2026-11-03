import struct, sys, threading, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

def movz(rd, imm, hw=0): return 0xD2800000 | (hw << 21) | (imm << 5) | rd
def movk(rd, imm, hw=0): return 0xF2800000 | (hw << 21) | (imm << 5) | rd
def movn(rd, imm):       return 0x92800000 | (imm << 5) | rd           # rd = ~imm
def adr(rd, off):        return 0x10000000 | ((off & 3) << 29) | (((off >> 2) & 0x7FFFF) << 5) | rd
SVC0 = 0xD4000001
MOV_X19_X0 = 0xAA0003F3; MOV_X0_X19 = 0xAA1303E0

code_words = [None]*14
# openat(AT_FDCWD, path, O_RDWR, 0)
code = [
    movz(8, 56),            # 0  x8 = openat
    movn(0, 99),            # 1  x0 = ~99 = -100 (AT_FDCWD)
    None,                   # 2  adr x1, path
    movz(2, 2),             # 3  x2 = O_RDWR
    movz(3, 0),             # 4  x3 = 0
    SVC0,                   # 5
    MOV_X19_X0,             # 6  x19 = fd
    movz(8, 29),            # 7  x8 = ioctl
    MOV_X0_X19,             # 8  x0 = fd
    movz(1, 0x4203),        # 9
    movk(1, 0x4004, 1),     # 10 x1 = 0x40044203
    None,                   # 11 adr x2, arg
    SVC0,                   # 12 ioctl(fd, cmd, &arg)  -> x0 = wynik
    movz(8, 93),            # 13 x8 = exit
    SVC0,                   # 14 exit(x0)
]
path_off = len(code) * 4
arg_off  = path_off + 12            # "/dev/hw_bfm\0" = 12 B
code[2]  = adr(1, path_off - 2*4)
code[11] = adr(2, arg_off - 11*4)
body = b"".join(struct.pack("<I", w) for w in code) + b"/dev/hw_bfm\0" + struct.pack("<I", 1)
# ELF64 AArch64, jeden PT_LOAD RWX
BASE = 0x400000; hdr_sz = 64 + 56
ehdr = b"\x7fELF" + bytes([2,1,1,0]) + b"\0"*8 + struct.pack("<HHIQQQIHHHHHH", 2, 183, 1, BASE+hdr_sz, 64, 0, 0, 64, 56, 1, 0, 0, 0)
phdr = struct.pack("<IIQQQQQQ", 1, 7, 0, BASE, BASE, hdr_sz+len(body), hdr_sz+len(body), 0x1000)
elf = ehdr + phdr + body
open(r"D:\y5_linux_work\bfmoff","wb").write(elf); print("ELF", len(elf), "B")

# wyslij na telefon
def listener(): y5sh.run("nc -l -p 2325 > /tmp/bfmoff; echo recv_done", timeout=40)
t = threading.Thread(target=listener); t.start(); time.sleep(2)
import socket
s = socket.create_connection(("172.16.42.1", 2325), timeout=5); s.sendall(elf); s.close(); t.join(30)
print(y5sh.run(r"""
chmod +x /tmp/bfmoff; ls -l /tmp/bfmoff
[ -e /dev/hw_bfm ] || mknod /dev/hw_bfm c 10 45
echo uptime_before: $(cut -d' ' -f1 /proc/uptime)
/tmp/bfmoff; echo exit_code=$?
dmesg | grep -i -E 'bfmr|disable.*timer|hwboot' | tail -n 6
""", timeout=40))
