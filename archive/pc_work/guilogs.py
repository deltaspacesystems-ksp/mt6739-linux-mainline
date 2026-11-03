import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- procesy:'; ps | grep -E 'Xvfb|weston|fbbridge|foot' | grep -v grep
for f in xvfb fbbridge weston foot; do echo "--- /tmp/$f.log:"; tail -n 6 /mnt/root/tmp/$f.log 2>&1; done
""", timeout=40))
