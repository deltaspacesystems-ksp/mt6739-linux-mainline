import json, urllib.request
req = urllib.request.Request("https://api.github.com/repos/NationalSecurityAgency/ghidra/releases/latest", headers={"User-Agent": "Mozilla/5.0"})
rel = json.load(urllib.request.urlopen(req, timeout=60))
sha_asset = next((a for a in rel["assets"] if a["name"].endswith(".sha256")), None)
print("plik sha256:", sha_asset["name"] if sha_asset else "brak")
if sha_asset:
    req2 = urllib.request.Request(sha_asset["browser_download_url"], headers={"User-Agent": "Mozilla/5.0"})
    print(urllib.request.urlopen(req2, timeout=60).read().decode().strip())
