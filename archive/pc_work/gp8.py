import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gpupresent", "/aroot/data/gpupresent.new", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/gpupresent.new
timeout 12 chroot /aroot /data/gpupresent.new 2>&1 </dev/null | tail -n 8
""", timeout=60))
