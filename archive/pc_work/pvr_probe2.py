import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- vendor/lib (32-bit):'; ls /ro/vendor/lib 2>&1 | grep -i -E 'egl|gles|pvr|gralloc|hwc'
echo '--- system/lib (32-bit):'; ls /ro/system/lib 2>&1 | grep -i -E 'egl|gles|pvr|gralloc|hwc'
echo '--- hw/ (hal modules) vendor:'; ls /ro/vendor/lib64/hw /ro/vendor/lib/hw 2>&1
echo '--- hw/ system:'; ls /ro/system/lib64/hw /ro/system/lib/hw 2>&1
echo '--- cale drzewo vendor/lib64 (pierwsze 40):'; ls /ro/vendor/lib64 2>&1 | head -n 40
echo '--- cale drzewo vendor/lib (pierwsze 40):'; ls /ro/vendor/lib 2>&1 | head -n 40
""", timeout=40))
