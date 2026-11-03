import sys, base64
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def put(path, text, mode="644"):
    b = base64.b64encode(text.replace("\r\n", "\n").encode()).decode()
    out = y5sh.run("mkdir -p $(dirname %s); : > %s" % (path, path), timeout=15)
    for i in range(0, len(b), 1200):
        y5sh.run("echo -n %s >> %s.b64" % (b[i:i+1200], path), timeout=15)
    return y5sh.run("base64 -d %s.b64 > %s; rm -f %s.b64; chmod %s %s; wc -c %s" % (path, path, path, mode, path, path), timeout=15).strip()
