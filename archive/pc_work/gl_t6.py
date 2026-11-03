import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg | grep -i -E 'PVR_K|RGX' | grep -v 'bad state' | head -n 30
echo; dmesg | grep -c 'bad state'
cat /sys/kernel/debug/pvr/status | head -n 3
""", timeout=60))
