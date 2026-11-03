import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- dmesg grep consys/combo/sdio po starcie wmt_launcher:'; dmesg | grep -i -E 'consys|combo|sdio|wmt' | tail -n 30
echo '--- clk consys:'; grep -i consys /sys/kernel/debug/clk/clk_summary
echo '--- strings: tabela -m (szukaj liczb kolo 'BT'/'WIFI'/'FM'/'GPS'):'
chroot /mnt/root python3 -c "
d=open('/ro/vendor/bin/wmt_launcher','rb').read()
import re
for m in re.finditer(rb'[ -~]{3,}', d):
    s=m.group().decode()
    if re.search(r'\b(BT|WIFI|GPS|FM)\b', s) and len(s)<80:
        print(s)
" | sort -u | head -n 40
""", timeout=40))
