import json, urllib.request, hashlib, re
req = urllib.request.Request("https://api.github.com/repos/NationalSecurityAgency/ghidra/releases/latest", headers={"User-Agent": "Mozilla/5.0"})
rel = json.load(urllib.request.urlopen(req, timeout=60))
print("tag:", rel["tag_name"])
asset = next(a for a in rel["assets"] if a["name"].endswith(".zip") and "PUBLIC" in a["name"])
print("asset:", asset["name"], asset["size"])
url = asset["browser_download_url"]
dst = r"D:\y5_linux_work\ghidra.zip"
req2 = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
with urllib.request.urlopen(req2, timeout=300) as r, open(dst, "wb") as f:
    while True:
        b = r.read(1<<20)
        if not b: break
        f.write(b)
import os
print("pobrano:", os.path.getsize(dst), "B")
h = hashlib.sha256(open(dst,"rb").read()).hexdigest()
print("sha256:", h)
