#!/bin/bash
set -eu
cd /home/y5/linux-7.3
grep -nE 'vldo28|VLDO28|VCAMA|VTOUCH' drivers/regulator/mt6357-regulator.c include/linux/mfd/mt6357/registers.h | head -30
grep CONFIG_INPUT_UINPUT .config
