import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
ls /proc | grep -i -E 'ged|pvr|gpu'; ls /sys/kernel/debug 2>&1 | tr '\n' ' '
echo; echo --- drm; ls /sys/class/drm; cat /sys/class/drm/card0/device/uevent 2>&1 | head
echo --- gpu clocks; ls /sys/kernel/debug/pvr /sys/kernel/debug/ged 2>&1 | head -n 20
cat /sys/kernel/debug/clk/clk_summary 2>/dev/null | grep -i -E 'mfg|gpu' | head
echo --- ged in kernel; grep -i -E ' ged_|pvr_drm|PVRSRV' /proc/kallsyms | head -n 8
echo --- chroot mounts; grep aroot /proc/mounts
""", timeout=30))
