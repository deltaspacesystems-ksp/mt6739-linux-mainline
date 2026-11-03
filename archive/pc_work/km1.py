import sys
sys.path.insert(0, r"D:\y5_linux_work")
import y5sh, pull
print(y5sh.run(r"""
cat /proc/uptime; uname -a; zcat /proc/config.gz | grep -E 'CONFIG_ARM64_VA_BITS|CONFIG_ARM64_64K|CONFIG_MODULE_FORCE_LOAD|CONFIG_MODVERSIONS|CONFIG_PREEMPT|CONFIG_SMP=|CONFIG_MODULE_SIG|CONFIG_RANDOMIZE_BASE|CONFIG_CPU_HOTPLUG|CONFIG_HOTPLUG_CPU'
grep -c . /proc/kallsyms
grep -E ' (printk|vmalloc_to_pfn|__get_free_pages|free_pages|local_irq_disable|__crc_module_layout|module_layout|memcpy|__flush_dcache_area|flush_icache_range|__cpu_flush_dcache_area|cpu_hotplug_disable) *$' /proc/kallsyms
cp /vendor/lib/modules/wmt_chrdev_wifi.ko /tmp/tmpl.ko 2>/dev/null; ls -l /aroot/vendor/lib/modules/
""", timeout=40))
print(pull.pull("/aroot/vendor/lib/modules/wmt_chrdev_wifi.ko", r"D:\y5_linux_work\tmpl.ko", port=2390))
print(pull.pull("/proc/kallsyms", r"D:\y5_linux_work\kallsyms.txt", port=2390))
