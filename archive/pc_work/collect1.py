import sys, os
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
os.makedirs(r"D:\y5_linux_work\dump", exist_ok=True)
def save(name, cmd, to=40):
    out = y5sh.run(cmd, timeout=to)
    open(os.path.join(r"D:\y5_linux_work\dump", name), "w", encoding="utf-8").write(out)
    print(name, len(out), "B")
save("clk_summary.txt", "cat /sys/kernel/debug/clk/clk_summary 2>&1", 30)
save("regulator_summary.txt", "cat /sys/kernel/debug/regulator/regulator_summary 2>&1", 30)
save("gpio.txt", "cat /sys/kernel/debug/gpio 2>&1", 30)
save("devices_deferred.txt", "cat /sys/kernel/debug/devices_deferred 2>&1", 15)
save("debugfs_ls.txt", "mount | grep debugfs; ls /sys/kernel/debug", 15)
save("kallsyms_head.txt", "head -n 5 /proc/kallsyms; wc -l /proc/kallsyms", 15)
