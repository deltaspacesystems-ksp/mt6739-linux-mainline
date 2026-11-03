import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gltest2", "/aroot/data/gltest2", port=2395))
print(y5sh.run("chmod +x /aroot/data/gltest2; pkill -9 gltest gltest2 2>/dev/null; ( timeout 25 chroot /aroot /data/gltest2 > /tmp/gl2.out 2>&1 < /dev/null & ); echo started", timeout=20))
