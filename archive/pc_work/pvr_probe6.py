import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- vendor/lib64/egl:'; ls -la /ro/vendor/lib64/egl
echo '--- vendor/lib/egl:'; ls -la /ro/vendor/lib/egl
echo '--- system/lib64/egl:'; ls -la /ro/system/lib64/egl 2>&1
echo '--- system/lib64 (libEGL/libGLES):'; ls /ro/system/lib64 | grep -i -E 'egl|gles'
echo '--- /dev/dri:'; ls -la /dev/dri
echo '--- /dev pvr*:'; ls -la /dev | grep -i pvr
echo '--- /sys/class/misc (pvr?):'; ls /sys/class/misc | grep -i -E 'pvr|rgx|srv'
echo '--- insmod'ow nie ma - PVRSRV jest wbudowane w kernel, wiec sprawdz /proc/pvr:'; ls /proc | grep -i pvr
""", timeout=40))
