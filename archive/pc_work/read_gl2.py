import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- gl2.out:'; cat /tmp/gl2.out
echo '--- proces:'; ps | grep gltest2 | grep -v grep
P=$(pidof gltest2 | awk '{print $1}')
[ -n "$P" ] && { echo '--- stat:'; cut -d' ' -f3,14,15 /proc/$P/stat; ls -l /proc/$P/fd 2>&1 | awk '{print $9,$10,$11}'; }
""", timeout=30))
