import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -l /aroot/vendor/firmware | grep -i rgx
mkdir -p /vendor; mount --bind /aroot/vendor /vendor 2>&1; ls /vendor/firmware | grep -i rgx
dmesg -c >/dev/null
( chroot /aroot /data/gltest2 >/tmp/gl6.out 2>&1 </dev/null & )
sleep 25; cat /tmp/gl6.out | tail -n 12
dmesg | grep -i -E 'PVR_K|RGX|pvrsrvkm' | head -n 15
""", timeout=80))
