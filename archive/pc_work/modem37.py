import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for c in 0x4301 0x4302 0x4303 0x430b 0x430c 0x430e; do
  dmesg -c >/dev/null
  r=$(chroot /mnt/root python3 /tmp/ci.py $c 2>&1 | tail -n 1)
  echo "== $c : $r"; sleep 1
  dmesg | grep -i -E 'ccci' | grep -v -E 'rbf\]|crb:|monitor' | head -n 4
  cat /sys/kernel/ccci/boot | cut -c1-6
done
""", timeout=60))
