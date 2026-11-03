import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
cat /sys/kernel/debug/pvr/status | head -n 30; cat /sys/kernel/debug/pvr/version
echo ---; ls /tmp/prev 2>&1 | head; grep -h -i -E 'PVR|RGX|mfg|gpu' /tmp/prev/* 2>/dev/null | head -n 40
echo --- ged; cat /sys/kernel/debug/ged/hal/* 2>/dev/null | head -n 10; cat /proc/gpufreq/gpufreq_opp_dump 2>&1 | head -n 8
""", timeout=60))
