import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\fbtest_arm64", "/tmp/fbtest", port=2332))
print(y5sh.run("/tmp/fbtest 2880 3", timeout=30))
