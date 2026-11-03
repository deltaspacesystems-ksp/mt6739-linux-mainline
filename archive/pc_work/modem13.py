import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
wc -l /tmp/mdtr.txt; ls -l /tmp/mstrace; grep -v 'sc2[0-9][0-9]()' /tmp/mdtr.txt | tail -n 40
""", timeout=40))
