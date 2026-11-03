import struct, sys, threading, time, socket
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def movz(rd, imm, hw=0): return 0xD2800000 | (hw << 21) | (imm << 5) | rd
def movk(rd, imm, hw=0): return 0xF2800000 | (hw << 21) | (imm << 5) | rd
def movn(rd, imm):       return 0x92800000 | (imm << 5) | rd
def adr(rd, off):        return 0x10000000 | ((off & 3) << 29) | (((off >> 2) & 0x7FFFF) << 5) | rd
SVC0 = 0xD4000001; MOV_X19_X0 = 0xAA0003F3; MOV_X0_X19 = 0xAA1303E0
code = [movz(8,56), movn(0,99), None, movz(2,2), movz(3,0), SVC0, MOV_X19_X0,
        movz(8,29), MOV_X0_X19, movz(1,0x4207), movk(1,0x4004,1), None, SVC0, movz(8,93), SVC0]
path_off = len(code)*4; arg_off = path_off + 12
code[2] = adr(1, path_off - 2*4); code[11] = adr(2, arg_off - 11*4)
body = b"".join(struct.pack("<I", w) for w in code) + b"/dev/hw_bfm\0" + struct.pack("<I", 0x7FFFFFFF)
BASE=0x400000; hdr=64+56
ehdr = b"\x7fELF"+bytes([2,1,1,0])+b"\0"*8+struct.pack("<HHIQQQIHHHHHH",2,183,1,BASE+hdr,64,0,0,64,56,1,0,0,0)
phdr = struct.pack("<IIQQQQQQ",1,7,0,BASE,BASE,hdr+len(body),hdr+len(body),0x1000)
elf = ehdr+phdr+body; open(r"D:\y5_linux_work\bfmok","wb").write(elf); print("ELF", len(elf))
def listener(): y5sh.run("nc -l -p 2327 > /tmp/bfmok; echo recv_done", timeout=40)
t = threading.Thread(target=listener); t.start(); time.sleep(2)
s = socket.create_connection(("172.16.42.1", 2327), timeout=5); s.sendall(elf); s.close(); t.join(30)
print(y5sh.run(r"""
chmod +x /tmp/bfmok
echo uptime_before: $(cut -d' ' -f1 /proc/uptime)
/tmp/bfmok; echo exit_code=$?
dmesg | grep -i -E 'bfm|hwboot' | tail -n 4
""", timeout=40))
