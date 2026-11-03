import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg -c >/dev/null
insmod /aroot/vendor/lib/modules/wmt_drv.ko 2>&1; echo insmod_rc=$?
sleep 2
grep -E '^(wmt|stp|bt|gps)' /proc/modules | cut -d' ' -f1,2
dmesg | grep -i -E 'wmt|stp|conn|mtk_wcn|combo|WMT|sdio' | head -n 25
grep -i -E 'wmt|stp|gps|bt|fm' /proc/devices
""", timeout=40))
