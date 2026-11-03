import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, y5put
print(y5put.put("/mnt/root/usr/local/bin/fbbridge.py", open(r"D:\y5_linux_work\guisrc\fbbridge.py", encoding="utf-8").read(), "755"))
print(y5sh.run("pkill python3; sleep 1; chroot /mnt/root /bin/sh -c 'python3 /usr/local/bin/fbbridge.py > /tmp/fbbridge.log 2>&1 &'; sleep 2; chroot /mnt/root sh -c 'ps' | grep python3", timeout=30))
