import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo MEMORY; cat /sys/kernel/debug/memblock/memory
echo RESERVED; cat /sys/kernel/debug/memblock/reserved
""", timeout=30))
