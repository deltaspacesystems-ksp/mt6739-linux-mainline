import re
t = open(r"D:\y5_linux_work\report.txt", encoding="utf-8").read()
secs = dict(re.findall(r"===== (.+?) =====\n(.*?)(?=\n===== |\Z)", t, re.S))
print("## net:", " ".join(secs["net"].split()))
print("\n## iomem (non-ccmni, kompletnie, pierwsze 70):")
print("\n".join(secs["iomem"].strip().split("\n")[:70]))
dm = secs["dmesg"].split("\n")
def g(pat, n=25):
    r = [l for l in dm if re.search(pat, l, re.I)]
    print(f"\n## dmesg /{pat}/ ({len(r)}):"); print("\n".join(r[:n]))
g(r"rndis|gadget|configfs|musb.*udc|usb0|android_usb|mtu3", 14)
g(r"mtkfb|fb_base|framebuffer|FB.*(addr|va|pa)|ion.*(fb|carveout)|lcm|panel", 14)
g(r"ramoops|pstore|reserve|cma|memblock|Reserved", 12)
