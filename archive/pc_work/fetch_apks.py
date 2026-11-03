"""Pobiera pakiety Alpine (aarch64) z listy i rozpakowuje do katalogu: python fetch_apks.py OUTDIR pkg1 pkg2 ...
Rozwiazuje zaleznosci (poza musl). Uzywane do dostarczania narzedzi na telefon (e2fsprogs itd.)."""
import urllib.request, io, tarfile, re, sys, zlib, os
BASE = "https://dl-cdn.alpinelinux.org/alpine/v3.24/main/aarch64/"
get = lambda u: urllib.request.urlopen(urllib.request.Request(u, headers={"User-Agent": "y5"}), timeout=120).read()
idx = tarfile.open(fileobj=io.BytesIO(get(BASE + "APKINDEX.tar.gz"))).extractfile("APKINDEX").read().decode()
pk, prov = {}, {}
for blk in idx.split("\n\n"):
    d = dict(re.findall(r"^([A-Za-z]):(.*)$", blk, re.M)) if blk.strip() else {}
    if "P" not in d: continue
    d["D"] = d.get("D", "").split(); pk[d["P"]] = d; prov[d["P"]] = d["P"]
    for p in d.get("p", "").split(): prov[p.split("=")[0]] = d["P"]
out = sys.argv[1]; need = sys.argv[2:]; seen = {}
while need:
    n = need.pop()
    if n in seen: continue
    seen[n] = pk[n]["V"]
    for dep in pk[n]["D"]:
        dep = re.split(r"[=<>~]", dep)[0]
        if dep.startswith("!"): continue
        t = prov.get(dep)
        if t and t not in seen and t not in ("musl", "scanelf"): need.append(t)
os.makedirs(out, exist_ok=True)
for n, v in seen.items():
    b = get(BASE + "%s-%s.apk" % (n, v)); streams = []
    while b:
        d = zlib.decompressobj(16 + zlib.MAX_WBITS); streams.append(d.decompress(b)); b = d.unused_data
    t = tarfile.open(fileobj=io.BytesIO(streams[-1])); t.extractall(out, filter="tar")
    print("ok", n, v)
