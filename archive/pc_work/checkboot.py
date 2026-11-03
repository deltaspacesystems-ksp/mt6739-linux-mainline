import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- /tmp/ready:'; cat /tmp/ready 2>&1
echo '--- chroot-up.log:'; cat /tmp/chroot-up.log 2>&1
echo '--- mountpoint:'; mountpoint /mnt/root 2>&1
echo '--- session.log (ostatnie):'; tail -n 10 /tmp/session.log 2>&1
echo '--- procesy w chroot:'; chroot /mnt/root sh -c 'ps' 2>&1
""", timeout=30))
