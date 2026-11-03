import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- /ro/vendor root:'; ls -la /ro/vendor
echo '--- /ro/system root:'; ls -la /ro/system
""", timeout=40))
