import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
print(y5sh.run(r"""
zcat /proc/config.gz | grep -E 'KEXEC|CRASH_DUMP|CONFIG_PSTORE|RAMOOPS|CONFIG_DEVMEM|CONFIG_STRICT_DEVMEM'
cat /proc/cmdline
grep -i -E 'ramoops|0x44410000|reserved' /proc/iomem | head
""", timeout=30))
