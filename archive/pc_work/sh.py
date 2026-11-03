import socket, sys, time, re
HOST, PORT = "172.16.42.1", 23
def run(cmd, timeout=15, host=HOST, port=PORT):
    s = socket.create_connection((host, port), timeout=5); s.settimeout(1.0)
    mark = "__DONE_%d__" % int(time.time()*1000)
    try:
        s.recv(4096)   # ewentualny banner / negocjacja telnetu
    except Exception: pass
    s.sendall((cmd + "\necho " + mark + "\n").encode())
    buf = b""; end = time.time() + timeout
    while time.time() < end:
        try:
            d = s.recv(65536)
            if not d: break
            buf += d
            if mark.encode() in buf.replace(b"echo " + mark.encode(), b""): break
        except socket.timeout: continue
    s.close()
    out = buf.decode(errors="replace")
    out = re.sub(r"\xff[\xfb-\xfe].", "", out)
    return out.split(mark)[0] if mark in out.replace("echo "+mark, "") else out
if __name__ == "__main__":
    print(run(" ".join(sys.argv[1:]) or "uname -a"))
