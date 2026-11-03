import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
print(y5put.put("/mnt/root/usr/local/bin/fbbridge.py", open(r"D:\y5_linux_work\guisrc\fbbridge.py", encoding="utf-8").read(), "755"))
print(y5put.put("/usr/local/bin/startgui2", open(r"D:\y5_linux_work\guisrc\startgui2.sh", encoding="utf-8").read(), "755"))
print(y5sh.run("/usr/local/bin/startgui2", timeout=90))
