import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
PID=$(pgrep -f 'dbus-daemon' | head -n1)
echo pid=$PID
cat /proc/$PID/environ 2>/dev/null | tr '\0' '\n' | grep DBUS
""", timeout=20))
