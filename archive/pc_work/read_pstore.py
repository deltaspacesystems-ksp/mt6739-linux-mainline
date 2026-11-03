import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run("echo up=$(cut -d' ' -f1 /proc/uptime); grep -o 'bootreason=[a-z_]*\\|boot_reason=[0-9]*' /proc/cmdline; ls -l /tmp/prev /sys/fs/pstore 2>&1", timeout=20))
out = y5sh.run("cat /tmp/prev/* 2>/dev/null | strings | head -c 400000", timeout=60)
open(r"D:\y5_linux_work\dump\pstore_prev.txt", "w", encoding="utf-8").write(out)
print("zapisano pstore_prev.txt:", len(out), "B")
