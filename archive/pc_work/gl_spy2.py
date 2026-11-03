import sys, time
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, push
print(push.push(r"D:\y5_linux_work\gltest", "/aroot/data/gltest", port=2380))
print(y5sh.run(r"""
chmod +x /aroot/data/gltest
pkill -9 gltest 2>/dev/null
( timeout 20 chroot /aroot /data/gltest > /tmp/gl.out 2>&1 & )
sleep 4
P=$(pidof gltest | awk '{print $1}')
echo pid=$P
echo '--- syscall (nr sp pc):'; cat /proc/$P/syscall 2>&1
echo '--- wchan:'; cat /proc/$P/wchan 2>&1; echo
echo '--- stat state/utime/stime:'; cut -d' ' -f3,14,15 /proc/$P/stat
echo '--- otwarte fd:'; ls -l /proc/$P/fd 2>&1 | awk '{print $9,$10,$11}'
echo '--- maps (biblioteki):'; awk '{print $6}' /proc/$P/maps | sort -u | head -n 30
echo '--- out:'; cat /tmp/gl.out
""", timeout=40))
