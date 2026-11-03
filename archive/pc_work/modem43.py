import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /sys/kernel/ccci/boot | cut -c1-6
dmesg | grep -E 'ccci1/(fsm|mcd)' | grep -v -E 'rbf|crb' | tail -n 15
ls /sys/class/net | grep ccmni | head -n 3; cat /sys/class/net/ccmni0/operstate 2>&1
""", timeout=40))
