import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /aroot
ls vendor/bin system/bin 2>/dev/null | grep -i -E 'ccci|nvram|rild|md_|mdlog|gsm0710|emdlog|mtkrild|muxreport|viarild|terservice|wmt|connsys|launcher' 
echo --- rc; grep -l -i -E 'ccci|md1|rild' vendor/etc/init/* system/etc/init/* vendor/etc/init/hw/* 2>/dev/null
echo --- md images; ls -l vendor/firmware 2>/dev/null | head -n 30
""", timeout=40))
