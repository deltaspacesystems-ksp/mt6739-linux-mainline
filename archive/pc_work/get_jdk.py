import json, urllib.request, hashlib, os
api = "https://api.adoptium.net/v3/assets/latest/21/hotspot?os=windows&arch=x64&image_type=jdk&vendor=eclipse"
req = urllib.request.Request(api, headers={"User-Agent": "Mozilla/5.0"})
d = json.load(urllib.request.urlopen(req, timeout=60))
pkg = next(x for x in d if x["binary"]["image_type"] == "jdk")["binary"]["package"]
url = pkg["link"]; sha = pkg["checksum"]
print("url:", url); print("sha256:", sha)
dst = r"D:\y5_linux_work\jdk21.zip"
req2 = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
data = urllib.request.urlopen(req2, timeout=120).read()
open(dst, "wb").write(data)
h = hashlib.sha256(data).hexdigest()
print("pobrano:", len(data), "B | sha256 zgodny:", h == sha)
