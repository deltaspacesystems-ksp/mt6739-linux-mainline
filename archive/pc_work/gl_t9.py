import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /proc/uptime; mountpoint /vendor; ls /dev/dri
( chroot /aroot /data/gltest2 >/tmp/gl7.out 2>&1 </dev/null & ); sleep 20; tail -n 12 /tmp/gl7.out
dmesg | grep -E 'PVR_K|pvrsrvkm' | head -n 8
""", timeout=70))
