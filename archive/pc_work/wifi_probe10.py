import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
mkdir -p /aroot/system /aroot/vendor /aroot/dev /aroot/proc /aroot/sys /aroot/data
mountpoint -q /aroot/system || mount --rbind /ro/system /aroot/system
mountpoint -q /aroot/vendor || mount --rbind /ro/vendor /aroot/vendor
mountpoint -q /aroot/dev || mount --rbind /dev /aroot/dev
mountpoint -q /aroot/proc || mount --rbind /proc /aroot/proc
mountpoint -q /aroot/sys || mount --rbind /sys /aroot/sys
ls /aroot/system/bin/linker64
echo '--- test uruchomienia (bez argumentow, zobaczymy blad):'
chroot /aroot /vendor/bin/wmt_launcher 2>&1 | head -n 20
""", timeout=40))
