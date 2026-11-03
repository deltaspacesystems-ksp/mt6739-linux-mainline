import re, sys
t = open(r"D:\y5_linux_work\report.txt", encoding="utf-8").read()
secs = dict(re.findall(r"===== (.+?) =====\n(.*?)(?=\n===== |\Z)", t, re.S))
def show(name, maxl=40, flt=None):
    body = secs.get(name, "(brak)").strip("\n").split("\n")
    if flt: body = [l for l in body if re.search(flt, l)]
    print(f"\n##### {name} ({len(body)} linii)")
    print("\n".join(body[:maxl]))
show("cmdline", 3)
show("iomem", 45, r"System RAM|Kernel|reserved|fb|disp|frame|ramoops|pstore|memory")
show("fb0", 10)
show("fb info", 8)
show("usb gadget (configfs)", 20)
show("udc", 6)
show("net", 6)
show("i2c devices", 20)
show("input", 30)
