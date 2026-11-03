import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -l /aroot/vendor/lib64/hw | grep -i -E 'gralloc|hwcomposer'; ls -l /aroot/vendor/lib/hw 2>&1| grep -i gralloc
""", timeout=30))
