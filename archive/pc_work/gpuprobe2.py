import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
def r(title, cmd, to=25):
    print("\n##", title); print(y5sh.run(cmd, timeout=to).strip()[:2200])
r("symbole PVR/RGX/GED w kallsyms (liczba)", "for p in PVRSRV RGX rgx_ pvr_ ged_ mtk_ged gpufreq mt_gpufreq; do echo $p: $(grep -c \"$p\" /proc/kallsyms); done")
r("przykladowe symbole", "grep -E ' (PVRSRVDriverInit|RGXInit|PVRSRVDeviceInitialise|ged_init|mt_gpufreq_init|mtk_gpufreq_init)$' /proc/kallsyms | head; grep -m6 -E 'PVRSRV' /proc/kallsyms")
r("dmesg: linie PVR/GED/gpufreq", "dmesg | grep -E 'PVR|RGX|\\[GED\\]|GPU DVFS|gpufreq|mfgsys|g3d|MFG' | head -n 25")
r("/proc/devices (znakowe, nie-standardowe)", "sed -n '1,40p' /proc/devices | tr '\\n' ' '")
r("dmesg: Mali/GPU irq", "grep -i -E 'gpu|mfg|g3d' /proc/interrupts | head")
r("gpufreq_state / power", "cat /proc/gpufreq/gpufreq_state /proc/gpufreq/gpufreq_power_dump 2>&1 | head -n 12")
