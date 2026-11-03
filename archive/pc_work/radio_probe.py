import sys, re
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
# 1) stockowy DTB: wezly lacznosci
t = open(r"D:\y5_linux_work\stock.dts.txt", encoding="utf-8").read().split("\n")
print("=== DTB: wezly gps/consys/wifi/bt/fm/ccci/sim/md ===")
seen = set()
for l in t:
    s = l.strip()
    if s.endswith("{") and re.search(r"gps|consys|wifi|wlan|btif|bluetooth|fm@|\bfm\b|ccci|md1|md_|sim|nfc|mnl", s, re.I):
        name = s[:-1].strip()
        if name not in seen: seen.add(name); print("  ", name)
# 2) dzialajacy system
cmd = r"""
echo '--- dmesg: WMT/consys/gps/wifi/bt/ccci/md:'; dmesg | grep -i -E 'wmt|consys|conninfra|MT66[0-9]+|MT6631|mt663|gps|connsys|ccci.*md1|modem|rild' | head -n 25
echo '--- /dev nodes lacznosci:'; ls /dev | grep -i -E 'gps|wmt|stp|ccci|ttyC|wlan|bt|fm|radio' | tr '\n' ' '
echo; echo '--- /proc/devices (lacznosc):'; grep -i -E 'wmt|stp|gps|ccci|btif|fm|mtk_btif|wlan' /proc/devices | tr '\n' ' '
echo; echo '--- net:'; ls /sys/class/net | grep -v -E 'ccmni|ifb|sit|tunl' | tr '\n' ' '
echo; echo '--- firmware/vendor (sciezka):'; grep -o 'firmware_class.path=[^ ]*' /proc/cmdline
echo '--- hardware sim:'; grep -o 'androidboot.hwsim=[a-z]*' /proc/cmdline
"""
print(y5sh.run(cmd, timeout=40))
