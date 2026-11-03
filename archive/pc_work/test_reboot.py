import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
try:
    print(y5sh.run("echo b > /proc/sysrq-trigger", timeout=5))
except Exception as e:
    print("restart wyslany (polaczenie zerwane jak oczekiwano):", type(e).__name__)
