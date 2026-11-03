import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\mstrace", "/tmp/mstrace", port=2395))
print(y5sh.run(r"""
chmod +x /tmp/mstrace
timeout 25 /tmp/mstrace /data/gltest2 >/tmp/gl4.out 2>/tmp/tr.txt </dev/null
tail -n 3 /tmp/gl4.out
grep -c . /tmp/tr.txt
grep -E 'sc56|ioctl' /tmp/tr.txt | grep -v -E '"/(system|vendor)/lib' | tail -n 50
""", timeout=60))
