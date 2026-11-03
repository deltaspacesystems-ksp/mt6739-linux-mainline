import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- dmesg PVR/RGX/ion/ged (ostatnie):'; dmesg | grep -i -E 'pvr|rgx|ion|ged|gpu|mfg|drm' | tail -n 25
echo '--- /dev (gpu-related):'; ls -l /dev | grep -i -E 'ion|ged|pvr|dri|graphics|mali|kgsl'
echo '--- /dev/dri:'; ls -l /dev/dri
echo '--- /sys/class/misc:'; ls /sys/class/misc | tr '\n' ' '
""", timeout=30))
