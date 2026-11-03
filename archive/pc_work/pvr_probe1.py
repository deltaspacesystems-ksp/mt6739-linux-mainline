import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- /proc/devices (szukaj pvr/rgx/ged/drm):'; grep -i -E 'pvr|rgx|ged|drm' /proc/devices
echo '--- /sys/class (pvr):'; ls /sys/class | grep -i -E 'pvr|rgx|ged|drm|graphics'
echo '--- /dev obecnie (pvr/dri):'; ls /dev | grep -i -E 'pvr|dri|rgx'
echo '--- binarki w /ro/vendor/bin zwiazane z gpu:'; ls /ro/vendor/bin | grep -i -E 'pvr|gpu|ged|surfaceflinger|rgx'
echo '--- biblioteki EGL/GLES w vendor/system (lib64):'; ls /ro/vendor/lib64 2>&1 | grep -i -E 'egl|gles|pvr' ; echo '---system lib64:'; ls /ro/system/lib64 2>&1 | grep -i -E 'egl|gles|pvr'
echo '--- firmware rgx:'; ls -la /ro/vendor/firmware/rgx.fw.signed
""", timeout=40))
