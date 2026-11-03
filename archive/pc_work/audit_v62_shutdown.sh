#!/bin/bash
set -eu
cd /home/y5/linux-7.3
find drivers/gpu/drm/mfw-rogue-ddk19 -name '*drm*' -o -name '*module*'
grep -R -nE '\.shutdown[[:space:]]*=|\.suppress_bind_attrs|\.remove[[:space:]]*=' drivers/gpu/drm/mfw-rogue-ddk19 | head -20
grep -n -A25 'smp_shutdown_nonboot_cpus' kernel/cpu.c | tail -30
grep -n 'CONFIG_KEXEC\|CONFIG_HOTPLUG_CPU\|CONFIG_DRM_MFW' .config
