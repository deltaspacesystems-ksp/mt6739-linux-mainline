import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\hello_arm64", "/tmp/hello", port=2331))
print(y5sh.run("/tmp/hello; echo exit=$?", timeout=15))
