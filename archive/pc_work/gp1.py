import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gpupresent", "/aroot/data/gpupresent", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/gpupresent
mkdir -p /aroot/xfb; mountpoint -q /aroot/xfb || mount --bind /mnt/root/tmp/xfb /aroot/xfb; ls /aroot/xfb
timeout 14 chroot /aroot /data/gpupresent 2>&1 </dev/null | tail -n 12
""", timeout=60))
