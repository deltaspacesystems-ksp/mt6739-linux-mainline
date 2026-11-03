import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -ld /aroot/system/vendor /aroot/system/vendor/lib64/egl 2>&1
mkdir -p /aroot/system/vendor 2>&1
mount --bind /aroot/vendor /aroot/system/vendor 2>&1
ls /aroot/system/vendor/lib64/egl 2>&1
timeout 20 chroot /aroot /data/gltest2 >/tmp/gl3.out 2>&1 </dev/null
tail -n 12 /tmp/gl3.out
""", timeout=60))
