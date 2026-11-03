#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '1,210p' drivers/power/supply/y5-pmic-monitor.c
sed -n '1,80p' drivers/regulator/mt6357-regulator.c
grep -nE 'gpu_id_init|CORE_ID|CORE_REVISION|PBVNC' drivers/gpu/drm/imagination/pvr_device.c | head -25
