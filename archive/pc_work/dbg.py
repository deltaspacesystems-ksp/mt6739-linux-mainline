import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo PATH=$PATH
ls -l /usr/local/bin/
echo '--- mkdevs:'; sh -x /usr/local/bin/mkdevs 2>&1 | tail -n 25
echo '--- /dev/input:'; ls -l /dev/input 2>&1 | head
echo '--- sysfs event3:'; ls /sys/class/input/ | head -n 12; cat /sys/class/input/event3/dev
""", timeout=40))
