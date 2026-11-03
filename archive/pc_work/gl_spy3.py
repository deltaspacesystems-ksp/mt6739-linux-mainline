import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
P=$(pidof gltest | awk '{print $1}')
echo pid=$P
echo '--- syscall (nr sp pc):'; cat /proc/$P/syscall 2>&1
echo '--- wchan:'; cat /proc/$P/wchan 2>&1; echo
echo '--- stat state/utime/stime:'; cut -d' ' -f3,14,15 /proc/$P/stat
echo '--- otwarte fd:'; ls -l /proc/$P/fd 2>&1 | awk '{print $9,$10,$11}'
echo '--- maps (biblioteki):'; awk '{print $6}' /proc/$P/maps | sort -u | head -n 30
echo '--- out:'; cat /tmp/gl.out
""", timeout=30))
