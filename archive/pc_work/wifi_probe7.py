import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
echo '--- /dev wmt*:'; ls -la /dev | grep -i -E 'wmt|stp|wifi'
echo '--- /proc wmt:'; ls /proc | grep -i wmt
echo '--- cat /proc/driver/wmt:'; cat /proc/driver/wmt 2>&1 | head -n 20
echo '--- /dev/wmtWifi probe (jesli jest):'; ls -la /dev/wmtWifi /dev/wmt0 /dev/stpwmt 2>&1
echo '--- firmware WMT_SOC.cfg w vendor/firmware:'; cat /ro/vendor/firmware/WMT_SOC.cfg 2>&1 | head -n 10
""", timeout=30))
