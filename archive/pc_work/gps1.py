import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -l /aroot/vendor/bin/mnld /aroot/vendor/bin/hw/*gnss* /aroot/vendor/bin/hw/*gps* 2>&1 | head
head -c 5 /aroot/vendor/bin/mnld | od -An -c | head -n 1
strings -a /aroot/vendor/bin/mnld | grep -E '^/(dev|data|vendor|system|sys|proc)' | sort -u | head -n 30
ls /aroot/vendor/firmware | head -n 40; ls /aroot/vendor/etc/gps* /aroot/vendor/etc/mtk* 2>/dev/null | head
""", timeout=40))
