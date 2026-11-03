import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls -l /vendor/firmware/rgx.fw.signed; cat /sys/module/firmware_class/parameters/path
dmesg | grep -E 'PVR_K|pvrsrvkm' | grep -v 'bad state' | tail -n 20
cat /sys/kernel/debug/pvr/status | head -n 3
grep -c . /tmp/gl7.out; tail -n 3 /tmp/gl7.out
ps | grep gltest | head -n 3
""", timeout=40))
