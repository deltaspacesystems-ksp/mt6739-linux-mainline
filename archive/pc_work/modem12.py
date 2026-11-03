import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
pkill ccci_mdinit; sleep 1
timeout 20 /tmp/mstrace /vendor/bin/ccci_mdinit 0 >/dev/null 2>/tmp/mdtr.txt </dev/null
grep -E 'sc56|sc48|ioctl|sc63|sc64' /tmp/mdtr.txt | grep -v -E '/lib|ld.config' | tail -n 45
""", timeout=60))
