import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /proc/iomem | head -n 60
echo ---; grep -E 'MemTotal|MemFree|CmaTotal|CmaFree' /proc/meminfo
echo ---; cat /sys/devices/system/cpu/online /sys/devices/system/cpu/possible
dmesg | grep -i -E 'memblock|reserved' | head -n 40
""", timeout=30))
