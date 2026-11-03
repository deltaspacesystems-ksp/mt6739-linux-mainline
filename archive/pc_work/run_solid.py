import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\fbsolid_arm64", "/tmp/fbsolid", port=2333))
print(y5sh.run("/tmp/fbsolid solid ff 00 00 ff", timeout=20))
