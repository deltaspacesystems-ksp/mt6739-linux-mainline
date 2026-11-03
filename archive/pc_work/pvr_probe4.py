import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mkdir -p /ro/system /ro/vendor
mountpoint -q /ro/vendor || mount -t ext4 -o ro /dev/part_vendor /ro/vendor
mountpoint -q /ro/system || mount -t ext4 -o ro /dev/part_system /ro/system
mount | grep '/ro/'
echo '--- /ro/vendor root:'; ls -la /ro/vendor
echo '--- /ro/system root:'; ls -la /ro/system
""", timeout=40))
