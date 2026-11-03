import urllib.request, zlib, io, tarfile, re, sys
base = "https://dl-cdn.alpinelinux.org/alpine/v3.24/main/aarch64/"
def get(u): return urllib.request.urlopen(urllib.request.Request(u, headers={"User-Agent": "y5"}), timeout=60).read()
def streams(b):
    out = []
    while b:
        d = zlib.decompressobj(16 + zlib.MAX_WBITS); out.append(d.decompress(b)); b = d.unused_data
    return out
idx = tarfile.open(fileobj=io.BytesIO(get(base + "APKINDEX.tar.gz"))).extractfile("APKINDEX").read().decode()
for blk in idx.split("\n\n"):
    if re.search(r"^P:busybox-extras$", blk, re.M):
        v = re.search(r"^V:(.+)$", blk, re.M).group(1); break
else: sys.exit("brak busybox-extras w indeksie")
print("wersja:", v)
apk = get(base + "busybox-extras-%s.apk" % v)
data = streams(apk)[-1]
t = tarfile.open(fileobj=io.BytesIO(data))
names = [m.name for m in t.getmembers()]
print([n for n in names if "busybox" in n or "telnetd" in n or "udhcpd" in n])
m = t.getmember("bin/busybox-extras")
open(r"D:\y5_linux_work\out\busybox-extras", "wb").write(t.extractfile(m).read())
print("zapisano", m.size)
