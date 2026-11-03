import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
echo '=== dostepnosc pakietow (apk search -x):'
for p in xorg-server xf86-video-fbdev xf86-input-evdev xf86-input-libinput xinit openbox matchbox-window-manager xterm st foot firefox firefox-esr epiphany netsurf surf chromium svkbd onboard matchbox-keyboard wpa_supplicant iw dhcpcd openssh dropbear font-dejavu mesa-dri-gallium e2fsprogs chrony ofono modemmanager gpsd gpsd-clients; do
  r=$(apk search -x $p 2>/dev/null | head -n 1); printf '%-26s %s\n' $p "${r:-BRAK}"
done
echo; echo '=== wejscia dotykowe/klawisze (/proc/bus/input/devices):'
grep -E '^N: Name|^H: Handlers' /proc/bus/input/devices | paste - - | sed 's/N: Name=//; s/H: Handlers=//'
echo; echo '=== partycje system/vendor (ro):'
for n in system vendor cust; do
  for b in /sys/class/block/*; do grep -qx "PARTNAME=$n" $b/uevent 2>/dev/null && { MM=$(cat $b/dev); [ -e /dev/part_$n ] || mknod /dev/part_$n b ${MM%:*} ${MM#*:}; echo "$n -> $(cat $b/size) sektorow"; }; done
done
mkdir -p /ro/system /ro/vendor
mount -t ext4 -o ro /dev/part_system /ro/system 2>&1 | tail -n 1; mount -t ext4 -o ro /dev/part_vendor /ro/vendor 2>&1 | tail -n 1
mount | grep ' /ro/'
echo '--- vendor: lacznosc:'; ls /ro/vendor/bin 2>/dev/null | grep -i -E 'wmt|gps|mnl|ccci|mtk|rild|wpa|mdlog' | tr '\n' ' '
echo; echo '--- vendor/firmware:'; ls /ro/vendor/firmware 2>/dev/null | head -n 30 | tr '\n' ' '
echo; echo '--- vendor/lib/modules:'; ls /ro/vendor/lib/modules /ro/vendor/lib64/modules 2>/dev/null | head -n 20 | tr '\n' ' '
echo; echo '--- system bin: linker64 / wpa:'; ls -l /ro/system/bin/linker64 /ro/system/bin/linker 2>&1 | tail -n 2
"""
print(y5sh.run(cmd, timeout=120))
