import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
pkill ccci_fsd; sleep 1
LD_PRELOAD=/data/propshim.so timeout 12 /tmp/mstrace /vendor/bin/ccci_fsd 0 >/dev/null 2>/tmp/fsdtr.txt </dev/null
wc -l /tmp/fsdtr.txt
grep -v -E 'logdw|pmsg0|c162|ld.config|/lib' /tmp/fsdtr.txt | head -n 50
echo ---; tail -n 5 /tmp/fsdtr.txt
""", timeout=40))
