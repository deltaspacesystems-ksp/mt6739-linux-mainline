import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
which strace ltrace 2>&1; ls /mnt/root/usr/bin/strace /aroot/system/bin 2>&1 | head -n 20
echo ---; ls /aroot/dev | head -n 30
echo ---; ls -l /aroot/vendor/lib64/egl /aroot/vendor/lib64/hw 2>&1 | head -n 30
""", timeout=30))
