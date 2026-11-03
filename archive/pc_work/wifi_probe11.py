import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
chroot /aroot /vendor/bin/wmt_launcher; echo "exit_code=$?"
echo '--- dmesg tail:'; dmesg | tail -n 15
echo '--- procesy wiszace:'; ps | grep wmt_launcher
""", timeout=40))
