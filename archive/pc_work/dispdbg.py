import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def r(t, c, to=25, n=1800):
    print("\n##", t); print(y5sh.run(c, timeout=to).strip()[:n])
r("typy wezlow", "for f in dispsys disp mtkfb fbconfig displowpower dfrc; do echo \"$f: $(ls -ld /sys/kernel/debug/$f | cut -c1-10) $(file_t=$(test -d /sys/kernel/debug/$f && echo dir || echo file))\"; done; ls /sys/kernel/debug/disp /sys/kernel/debug/mtkfb 2>&1 | head -n 20")
r("mtkfb debugfs (czytaj)", "cat /sys/kernel/debug/mtkfb 2>&1 | head -n 40")
r("dispsys: dump_reg", "echo 'dump_reg:0' > /sys/kernel/debug/dispsys 2>&1; cat /sys/kernel/debug/dispsys 2>&1 | head -n 60")
r("sysfs mtkfb", "ls /sys/devices/platform/mtkfb /sys/class/graphics/fb0 2>&1 | head -n 40")
r("dmesg: OVL/pitch/addr", "dmesg | grep -i -E 'ovl|pitch|layer|fb_pa|mva|framebuffer|lcm' | head -n 25")
