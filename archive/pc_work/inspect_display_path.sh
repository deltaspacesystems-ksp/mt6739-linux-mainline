#!/bin/bash
set -eu
cd /home/y5/linux-7.3
ls Module.symvers vmlinux.symvers 2>/dev/null || true
grep -E 'CONFIG_MODULES=|CONFIG_MODVERSIONS=|CONFIG_MODULE_SIG' .config || true
cat drivers/video/y5disp.c
grep -nE 'y5|775|fbdev|copy|dirty|update|screen_base|iosys_map' drivers/gpu/drm/sysfb/drm_sysfb_modeset.c | head -55
