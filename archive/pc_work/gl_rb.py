import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
try: print(y5sh.run("sync; echo 1 > /proc/sys/kernel/sysrq; echo b > /proc/sysrq-trigger", timeout=8))
except Exception as e: print("sent", e)
