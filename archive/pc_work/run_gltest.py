import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gltest", "/aroot/data/gltest", port=2370))
print(y5sh.run(r"""
chmod +x /aroot/data/gltest
chroot /aroot /data/gltest
echo exit_code=$?
""", timeout=40))
