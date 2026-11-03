#!/bin/bash
set -eu
cd /home/y5/linux-7.3
sed -n '50,112p' drivers/regulator/mt6357-regulator.c
sed -n '228,246p' drivers/regulator/mt6357-regulator.c
grep -n 'vldo28_idx' drivers/regulator/mt6357-regulator.c
