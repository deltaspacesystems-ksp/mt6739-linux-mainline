import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
for i in 0 1 2; do
  dd if=/tmp/pat.raw of=/dev/fb0 bs=4096 seek=$(expr $i \* 1035) 2>&1 | tail -n 1
done
echo frames_done
"""
print(y5sh.run(cmd, timeout=30))
