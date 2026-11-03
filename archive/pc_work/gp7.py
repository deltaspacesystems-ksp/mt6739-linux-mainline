import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /sys/class/graphics/fb0; for f in virtual_size bits_per_pixel stride mode modes name; do echo "$f: $(cat $f 2>&1 | head -n 2)"; done
tail -n 4 /tmp/gpup.out
""", timeout=30))
