import sys, socket, threading, time, zlib, struct
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def shot(png=r"D:\y5_linux_work\screen.png"):
    W, H, S = 720, 1440, 2944
    out = []
    def recv():
        s = socket.socket(); s.bind(("172.16.42.2", 2399)); s.listen(1); s.settimeout(40)
        c, _ = s.accept(); buf = b""
        while len(buf) < S * H:
            d = c.recv(1 << 20)
            if not d: break
            buf += d
        out.append(buf); c.close(); s.close()
    t = threading.Thread(target=recv); t.start(); time.sleep(0.5)
    y5sh.run("dd if=/dev/fb0 bs=%d count=1 2>/dev/null | nc 172.16.42.2 2399" % (S * H), timeout=40)
    t.join(45)
    raw = out[0][:S * H]
    rows = b"".join(b"\x00" + b"".join(raw[y * S + x * 4: y * S + x * 4 + 3] for x in range(W)) for y in range(H))
    def chunk(t_, d): c = struct.pack(">I", len(d)) + t_ + d; return c + struct.pack(">I", zlib.crc32(t_ + d) & 0xffffffff)
    open(png, "wb").write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", W, H, 8, 2, 0, 0, 0)) + chunk(b"IDAT", zlib.compress(rows, 6)) + chunk(b"IEND", b""))
    return len(raw)
if __name__ == "__main__":
    print("bajtow:", shot())
