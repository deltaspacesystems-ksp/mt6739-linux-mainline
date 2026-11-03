import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def r(title, cmd, to=25):
    print("\n##", title); print(y5sh.run(cmd, timeout=to).strip()[:2500])
r("wezly /dev (gpu)", "ls /dev | grep -i -E 'pvr|mali|ged|gpu|kgsl|dri|ion|fb' ; ls /dev/dri 2>&1 | head")
r("dmesg gpu", "dmesg | grep -i -E 'pvr|rogue|powervr|\\bged\\b|mfg|mali|gpufreq|gpu' | head -n 30")
r("zegary mfg/gpu", "grep -i -E 'mfg|gpu|mali' /sys/kernel/debug/clk/clk_summary | head -n 15")
r("regulator gpu", "grep -i -E 'vgpu|vcore|mfg|gpu' /sys/kernel/debug/regulator/regulator_summary | head -n 10")
r("gpufreq proc", "ls /proc/gpufreq /sys/kernel/debug/ged 2>&1 | head -n 30; cat /proc/gpufreq/gpufreq_opp_freq 2>&1 | head; cat /proc/gpufreq/gpufreq_opp_dump 2>&1 | head -n 12")
r("iomem gpu (mfg)", "grep -i -E '130|mfg|g3d|gpu' /proc/iomem | head")
r("moduly / sterowniki PVR", "grep -i -E 'pvr|rgx|ged|mali' /proc/modules /proc/devices /proc/kallsyms 2>/dev/null | head -n 12")
