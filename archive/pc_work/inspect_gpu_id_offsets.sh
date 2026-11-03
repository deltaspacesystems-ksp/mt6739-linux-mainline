#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '415,443p' drivers/gpu/drm/imagination/pvr_device.c
grep -n '^#define ROGUE_CR_CORE_ID\|^#define ROGUE_CR_CORE_REVISION' drivers/gpu/drm/imagination/pvr_rogue_cr_defs.h | head -25
sed -n '465,535p' drivers/power/supply/y5-pmic-monitor.c
