#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '55,120p' drivers/gpu/drm/mfw-rogue-ddk19/kernel/drivers/staging/imgtec/pvr_platform_drv.c
find drivers/iio -iname '*ltr*' -o -iname '*mc3*'
grep -n 'compatible' drivers/i2c/busses/i2c-mt65xx.c | tail -18
tail -n 4 /home/y5/build73f.log
