import socket, threading, time, sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh

def start(port=2399, outfile=r"D:\y5_linux_work\live_dmesg.log"):
    f = open(outfile, "ab", buffering=0)
    def listener():
        s = socket.socket(); s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("172.16.42.2", port)); s.listen(1)
        while True:
            try:
                s.settimeout(None)
                c, _ = s.accept()
                c.settimeout(2)
                while True:
                    try:
                        d = c.recv(4096)
                        if not d: break
                        f.write(d)
                    except socket.timeout:
                        continue
            except Exception:
                time.sleep(0.5)
    t = threading.Thread(target=listener, daemon=True); t.start()
    time.sleep(0.5)
    y5sh.run("( while true; do dmesg -c 2>&1; sleep 0.1; done | nc 172.16.42.2 %d & ) ; echo started" % port, timeout=10)
    return outfile

if __name__ == "__main__":
    f = start()
    print("live log -> ", f)
