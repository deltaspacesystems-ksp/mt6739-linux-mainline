import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gltest2", "/aroot/data/gltest2", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/gltest2; cat /proc/uptime
dmesg | grep -E 'PVR_K.*Error|pvrsrvkm' | head -n 6
( chroot /aroot /data/gltest2 >/tmp/gl8.out 2>&1 </dev/null & ); sleep 15; tail -n 14 /tmp/gl8.out
""", timeout=50))
