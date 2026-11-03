import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /sys/class/drm/renderD128/dev /sys/class/drm/controlD64/dev
mknod /dev/dri/renderD128 c 226 128 2>&1; mknod /dev/dri/controlD64 c 226 64 2>&1; ls -l /dev/dri /aroot/dev/dri
dmesg -c >/dev/null 2>&1
timeout 20 chroot /aroot /data/gltest2 >/tmp/gl3.out 2>&1 </dev/null
tail -n 12 /tmp/gl3.out
echo --- dmesg; dmesg | grep -i -E 'pvr|rgx|ged|drm|ion' | tail -n 20
""", timeout=60))
