import re, sys
lines = open(r"D:\y5_linux_work\stock.dts.txt").read().split("\n")
# zbierz wezly: sciezka, compatible, reg
stack=[]; nodes=[]; cur=None
for ln in lines:
    s = ln.strip()
    if s.endswith("{"):
        name = s[:-1].strip(); stack.append(name); cur = {"path":"/".join(stack), "compat":None, "reg":None, "depth":len(stack)}; nodes.append(cur)
    elif s == "};":
        stack.pop(); cur = nodes[-1] if False else None
    elif cur is not None and s.startswith("compatible"):
        cur["compat"] = s.split("=",1)[1].strip(" ;")
    elif cur is not None and s.startswith("reg ="):
        cur["reg"] = s.split("=",1)[1].strip(" ;")[:60]
def show(title, pat):
    print("\n##", title)
    n=0
    for x in nodes:
        if x["compat"] and re.search(pat, x["compat"], re.I) and n<14:
            print(f'  {x["path"].split("/")[-1][:34]:34} {x["compat"][:70]:70} {x["reg"] or ""}'); n+=1
show("UART",       r"uart|serial")
show("IRQ/timer",  r"gic|timer|psci|cpu")
show("MMC/eMMC",   r"mmc|msdc|sd")
show("USB",        r"usb|musb|mtu3|xhci")
show("Display",    r"disp|dsi|mtkfb|lcm|panel|mdp|ovl|rdma|dpi")
show("PMIC/power", r"pmic|regulator|charger|battery|rtc|pwrap|keys|keypad")
show("Clocks",     r"topckgen|infracfg|apmixed|clk|scpsys|pericfg")
show("Pinctrl/gpio", r"pinctrl|gpio|eint")
show("Touch/I2C/SPI", r"i2c|spi|touch|tp|accel|sensor|alsps")
show("Memory-ish", r"reserved|ramoops|pstore|memory|ion")
print("\nvsego wezlow:", len(nodes))
