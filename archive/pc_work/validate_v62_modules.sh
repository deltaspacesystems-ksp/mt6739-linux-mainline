#!/bin/bash
set -eu
cd /home/y5/linux-7.3
aarch64-linux-gnu-nm vmlinux | grep -E ' (sys_kexec_file_load|__arm64_sys_kexec_file_load|machine_kexec|smp_shutdown_nonboot_cpus)$'
find /mnt/d/y5_linux_work/out/v62-module-root/lib/modules/7.3.0 -name '*.ko' | wc -l
tar -C /mnt/d/y5_linux_work/out/v62-module-root -czf /mnt/d/y5_linux_work/out/mfw-v62-modules.tar.gz lib
sha256sum /mnt/d/y5_linux_work/out/mfw-v62-modules.tar.gz
