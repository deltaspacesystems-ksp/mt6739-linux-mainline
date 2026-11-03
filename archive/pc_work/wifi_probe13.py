import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- strings wmt_launcher (usage/help):'
chroot /mnt/root python3 -c "
d=open('/ro/vendor/bin/wmt_launcher','rb').read()
import re
for m in re.finditer(rb'[ -~]{5,}', d):
    s=m.group().decode()
    if any(k in s.lower() for k in ('usage','wlan','wifi','option','-p','daemon','cmd','func')):
        print(s)
" | sort -u | head -n 60
""", timeout=40))
