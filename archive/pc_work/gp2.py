import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gpupresent_bench", "/aroot/data/gpupresent_bench", port=2395))
print(y5sh.run(r"""
chmod +x /aroot/data/gpupresent_bench
timeout 12 chroot /aroot /data/gpupresent_bench 2>&1 </dev/null | tail -n 4
""", timeout=60))
