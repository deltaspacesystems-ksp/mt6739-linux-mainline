#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -n -A 30 'static struct regulator \*_regulator_get\|struct regulator \*_regulator_get' drivers/regulator/core.c | head -40
grep -nE 'device_link_add|device_lock\(' drivers/regulator/core.c | head -15
