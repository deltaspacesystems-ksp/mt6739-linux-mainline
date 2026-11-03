import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ps | grep -E 'ccci|nvram' | grep -v -E 'grep|\[ccci_(fsm|poll|rpc|ipc|sys|ctrl)'
cat /tmp/fsd.out /tmp/rpcd.out /tmp/nvram.out | head -n 20
ls /aroot/vendor/nvdata/md /aroot/vendor/nvdata/md_cmn /aroot/vendor/protect_f/md 2>&1 | head -n 20
echo --- dmesg 1301-1336 non-rbf; dmesg | grep -v -E 'rbf\]|crb:|VcoreFS|charger|Power/|fgauge|Batt|JEITA|PPM|USB_BOOST|ESD|wdk|usb_state|gadget|mcdi' | awk -F'[][]' '$2+0>1301 && $2+0<1337' | head -n 60
""", timeout=40))
