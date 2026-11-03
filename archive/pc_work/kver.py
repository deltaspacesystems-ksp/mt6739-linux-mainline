import json, urllib.request
d = json.load(urllib.request.urlopen("https://www.kernel.org/releases.json", timeout=30))
print("latest_stable:", d["latest_stable"]["version"])
for r in d["releases"][:8]:
    print(" ", r["moniker"].ljust(10), r["version"].ljust(14), r["released"]["isodate"], r.get("iseol", False) and "EOL" or "")
