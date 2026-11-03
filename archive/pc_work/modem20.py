import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cd /aroot/vendor
grep -l -a -E 'ccci/boot|ccci_md_sta|CCCI_IOC_SEND_RUN_TIME|ccci_ioctl' bin/* bin/hw/* lib/*.so lib64/*.so 2>/dev/null | head -n 20
echo ---; strings -a bin/md_ctrl | grep -i -E '^/|ccci|boot|md' | head -n 20
echo --- dmesg tail ccci; dmesg | grep -i -E 'ccci|md1' | tail -n 15
""", timeout=40))
