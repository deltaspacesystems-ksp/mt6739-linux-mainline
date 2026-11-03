import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg -c >/dev/null 2>&1
chroot /aroot /system/bin/linker64 /data/gltest2 >/tmp/gl3.out 2>&1 &
sleep 8
cat /tmp/gl3.out | tail -n 5
echo --- dmesg; dmesg | tail -n 40
echo --- strings; strings /aroot/vendor/lib64/egl/libEGL_mtk.so | grep -i -E '\.so|/dev|/vendor|prop|ro\.' | head -n 30
""", timeout=40))
