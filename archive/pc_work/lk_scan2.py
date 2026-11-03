d = open(r"D:\huawei_y5_backup\lk.bin","rb").read()
def near(off, span=120):
    s = max(0, off-span); e = min(len(d), off+span)
    chunk = d[s:e]
    import re
    strs = re.findall(rb"[ -~]{4,}", chunk)
    return [x.decode() for x in strs]
for off in (386047, 391920, 392717, 393840, 394076, 396643, 396662, 499489, 519308, 526769, 529278, 545228):
    print(f"--- @ {off} ---")
    for s in near(off): print("  ", s)
