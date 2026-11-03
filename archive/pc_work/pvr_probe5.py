import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- vendor/lib64 (egl/gles/pvr/gralloc/hw):'; ls /ro/vendor/lib64 | grep -i -E 'egl|gles|pvr|gralloc|hw|gpu'
echo '--- vendor/lib64/hw:'; ls /ro/vendor/lib64/hw 2>&1
echo '--- vendor/lib (32-bit, egl/gles/pvr):'; ls /ro/vendor/lib | grep -i -E 'egl|gles|pvr|gralloc'
echo '--- vendor/bin (pvr/gpu/surface):'; ls /ro/vendor/bin | grep -i -E 'pvr|gpu|surface|rgx|ged'
echo '--- vendor/bin pelna lista:'; ls /ro/vendor/bin
""", timeout=40))
