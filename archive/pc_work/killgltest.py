import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("pkill -9 gltest; sleep 1; ps | grep gltest; echo done", timeout=20))
