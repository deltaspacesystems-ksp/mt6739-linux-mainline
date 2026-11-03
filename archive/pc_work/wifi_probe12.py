import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("ps | grep wmt_launcher; echo ---; dmesg | tail -n 15", timeout=20))
