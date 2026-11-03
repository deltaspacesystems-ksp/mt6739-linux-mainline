import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
timeout 12 /tmp/mstrace /data/gltest2 >/dev/null 2>/tmp/tr2.txt </dev/null
grep -E 'sc56|sc48|ioctl' /tmp/tr2.txt | tail -n 30
echo ---; tail -n 8 /tmp/tr2.txt
""", timeout=60))
