import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
dmesg -c >/dev/null 2>&1
timeout 15 chroot /aroot /data/gltest2 >/tmp/gl3.out 2>&1 </dev/null
tail -n 4 /tmp/gl3.out
echo --- dmesg; dmesg | grep -v -E 'charger|fan54005|SW_JEITA|Power/|VcoreFS|USB_BOOST|mcdi|gadget|ESD|usb_state|Batt|fgauge' | tail -n 30
echo --- strings; for f in libIMGegl libsrv_um libgralloc_extra; do echo == $f; strings /aroot/vendor/lib64/$f.so | grep -E '^/|\.so|ro\.|persist\.|debug\.' | head -n 25; done
""", timeout=60))
