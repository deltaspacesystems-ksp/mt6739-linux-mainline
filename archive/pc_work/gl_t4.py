import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
grep -n -E 'dri/(card0|controlD64|renderD128)' /tmp/tr2.txt
grep -n -B3 -A12 'renderD128' /tmp/tr2.txt | head -n 40
dmesg | grep -i -E 'pvr|drm' | tail -n 15
""", timeout=60))
