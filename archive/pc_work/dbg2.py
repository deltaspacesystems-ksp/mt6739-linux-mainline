import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("head -c 300 /usr/local/bin/startgui2 | od -c | head -n 6; echo ---; sh -x /usr/local/bin/startgui2 2>&1 | head -n 40", timeout=90))
