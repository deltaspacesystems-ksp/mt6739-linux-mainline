import json, urllib.request, hashlib, os, re, sys
d = json.load(urllib.request.urlopen("https://ziglang.org/download/index.json", timeout=60))
def key(v): return tuple(int(x) for x in re.findall(r"\d+", v)[:3])
vers = sorted([k for k in d if k != "master" and re.fullmatch(r"\d+\.\d+\.\d+", k)], key=key, reverse=True)
print("najnowsze stabilne:", vers[:4])
v = vers[0]; e = d[v]["x86_64-windows"]
print("wybrano", v, e["tarball"], e["size"], "B", "sha256", e["shasum"])
dst = os.path.join(r"D:\y5_linux_work", os.path.basename(e["tarball"]))
urllib.request.urlretrieve(e["tarball"], dst)
h = hashlib.sha256(open(dst, "rb").read()).hexdigest()
print("pobrano", os.path.getsize(dst), "B | sha256 zgodny:", h == e["shasum"])
