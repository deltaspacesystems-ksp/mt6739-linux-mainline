import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh
out = y5sh.run(r"""
uname -a; zcat /proc/config.gz | grep -E 'CONFIG_ARM64_VA_BITS|CONFIG_ARM64_64K|CONFIG_MODULE_FORCE_LOAD|CONFIG_MODVERSIONS|CONFIG_PREEMPT|CONFIG_SMP=|CONFIG_MODULE_SIG|CONFIG_RANDOMIZE_BASE|CONFIG_HOTPLUG_CPU|CONFIG_ARM64_PAGE_SHIFT'
grep -E ' (printk|vmalloc_to_pfn|__get_free_pages|free_pages|__crc_module_layout|module_layout|__flush_dcache_area|flush_icache_range|__memcpy|memcpy|msleep|mdelay)$' /proc/kallsyms
""", timeout=40)
print(out)
