#!/bin/bash
set -eu
cd /home/y5/linux-7.3
find drivers -name '*y5*pmic*' -o -name '*mfw*pmic*'
grep -nE 'PMIC_WRAP|MT6357|REGULATOR|PM_GENERIC_DOMAINS' .config
sed -n '1,110p' drivers/pmdomain/mediatek/mt6735-pm-domains.h
