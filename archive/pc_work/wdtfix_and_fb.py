import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
cmd = r"""
ls -l /dev/watchdog* 2>&1; cat /proc/devices | grep -i -E 'watchdog|misc'
cat /sys/class/misc/watchdog/dev 2>&1; ls /sys/class/watchdog 2>&1
cat /sys/kernel/debug/mtkfb | grep -E 'xres|linebytes|offset|Framebuffer'
"""
print(y5sh.run(cmd, timeout=30))
