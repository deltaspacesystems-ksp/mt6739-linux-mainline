import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
grep -v -E 'logdw|pmsg0|c162|ld.config|/lib' /tmp/mdtr.txt | grep -E '"/|c54\(' | head -n 60
""", timeout=40))
