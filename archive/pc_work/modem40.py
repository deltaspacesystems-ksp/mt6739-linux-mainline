import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg | grep -E 'ccci1/chr' | grep -E 'open|close' | head -n 30
echo ---; dmesg | grep -i -E 'HS2|exception|boot_stats|Dump' | head; cat /sys/kernel/ccci/boot | cut -c1-6
""", timeout=40))
