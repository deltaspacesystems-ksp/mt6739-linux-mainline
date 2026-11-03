import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
timeout 20 /tmp/mstrace /data/vktest >/tmp/vk.out 2>/tmp/vktr.txt </dev/null
grep -E 'sc56|ioctl' /tmp/vktr.txt | grep -v -E '"/(system|vendor)/lib|ld.config' | tail -n 25
""", timeout=40))
