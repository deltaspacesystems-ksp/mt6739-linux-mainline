import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gltest2", "/aroot/data/gltest2", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/gltest2
cat /sys/kernel/debug/pvr/status | head -n 3
dmesg | grep -E 'PVR_K.*Error|firmware' | head -n 8
( chroot /aroot /data/gltest2 >/tmp/gl7.out 2>&1 </dev/null & ); sleep 20; tail -n 14 /tmp/gl7.out
dmesg | grep -E 'PVR_K.*Error' | tail -n 5
""", timeout=70))
