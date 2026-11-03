#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -nE 'fdt|initrd|cmdline|memblock|reserved' arch/arm64/kernel/machine_kexec_file.c | head -45
grep -n -A35 'of_kexec_alloc_and_setup_fdt' drivers/of/kexec.c | head -55
grep -n -A25 'PM_SLEEP_SMP' kernel/power/Kconfig
