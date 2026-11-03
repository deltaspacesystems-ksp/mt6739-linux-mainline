p = "/root/icons.py"
s = open(p).read()
old = '    if kind == "wifi":\n        return WifiIcon(size=size, color=color)\n'
assert old in s and 'kind == "cellular"' not in s
s = s.replace(old, old + '    if kind == "cellular":\n        return CellularIcon(level=level, size=size, color=color)\n', 1)
open(p, "w").write(s)
print("patched")
