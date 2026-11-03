import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
t0=time.time()
while time.time()-t0 < 120:
    try:
        r = y5sh.run(r"""
printf '#!/bin/sh\nexit 0\n' > /usr/local/bin/wcn-up; chmod 755 /usr/local/bin/wcn-up
pkill -f wcn-up; pkill wmt_loader; pkill wmt_launcher
cat /proc/uptime; ps | grep -E 'wcn-up|wmt_' | grep -v grep
""", timeout=6)
        print(r); break
    except Exception as e:
        time.sleep(0.3)
