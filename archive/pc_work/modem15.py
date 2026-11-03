import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\mstrace", "/tmp/mstrace", port=2395))
print(y5sh.run(r"""
chmod +x /tmp/mstrace; pkill ccci_mdinit
timeout 15 /tmp/mstrace /vendor/bin/ccci_mdinit 0 >/dev/null 2>/tmp/mdtr.txt </dev/null
wc -l /tmp/mdtr.txt
grep -E '"/' /tmp/mdtr.txt | grep -v -E '"/(system|vendor)/lib|ld.config' | tail -n 30
echo ---; tail -n 8 /tmp/mdtr.txt
""", timeout=60))
