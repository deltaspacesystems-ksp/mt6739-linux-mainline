import sys, threading, time, socket
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

def pull(remote, local, port=2360):
    out = {}
    def listener():
        s = socket.socket(); s.bind(("172.16.42.2", port)); s.listen(1); s.settimeout(30)
        c, _ = s.accept(); buf = b""
        c.settimeout(10)
        try:
            while True:
                d = c.recv(1 << 20)
                if not d: break
                buf += d
        except socket.timeout:
            pass
        out["data"] = buf; c.close(); s.close()
    t = threading.Thread(target=listener); t.start(); time.sleep(1.0)
    y5sh.run("nc 172.16.42.2 %d < %s" % (port, remote), timeout=30)
    t.join(35)
    data = out.get("data", b"")
    open(local, "wb").write(data)
    return len(data)

if __name__ == "__main__":
    n = pull(sys.argv[1], sys.argv[2])
    print(n, "B ->", sys.argv[2])
