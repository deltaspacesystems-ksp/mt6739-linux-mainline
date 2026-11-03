import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
pkill -9 wmt_launcher 2>/dev/null; sleep 1
rm -f /tmp/wmtl.log
nohup chroot /aroot /vendor/bin/wmt_launcher -m 3 -p /vendor/firmware/ > /tmp/wmtl.log 2>&1 &
sleep 3
echo '--- log:'; cat /tmp/wmtl.log
echo '--- procesy:'; ps | grep wmt_launcher
echo '--- net:'; ls /sys/class/net | grep -v ccmni
""", timeout=40))
