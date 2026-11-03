#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -nE 'VCORE|vcore|pwrap|static.*read|mutex|_show' drivers/power/supply/y5-pmic-monitor.c | head -48
grep -nE 'vcore|VCORE' drivers/regulator/mt6357-regulator.c include/linux/mfd/mt6357/registers.h | head -32
grep -n '^CONFIG_PM\|PM_GENERIC\|CONFIG_OF=' .config
