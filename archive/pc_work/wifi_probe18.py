import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /mnt/root python3 -c "
import re
d=open('/ro/vendor/lib/modules/wmt_chrdev_wifi.ko','rb').read()
for m in re.finditer(rb'[ -~]{4,}', d):
    s=m.group().decode()
    if re.search(r'ioctl|IOCTL|cmd|func_ctrl|open|release|WIFI', s, re.I):
        print(s)
" | sort -u
""", timeout=40))
