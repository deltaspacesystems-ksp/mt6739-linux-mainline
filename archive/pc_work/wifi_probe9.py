import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
for c in wmtWifi wmtdetect; do
  MM=$(cat /sys/class/$c/$c/dev)
  [ -e /dev/$c ] || mknod /dev/$c c ${MM%:*} ${MM#*:}
done
ls -la /dev/wmtWifi /dev/wmtdetect
echo '--- wmt_launcher z vendor:'; ls -la /ro/vendor/bin/wmt_launcher 2>&1
file /ro/vendor/bin/wmt_launcher 2>&1
echo '--- biblioteki potrzebne (wymagaja bionic linker):'; chroot /mnt/root python3 -c "
d=open('/ro/vendor/bin/wmt_launcher','rb').read()
i=d.find(b'/system/bin/linker')
print('interp:', d[i:i+40] if i>0 else 'brak')
"
""", timeout=40))
