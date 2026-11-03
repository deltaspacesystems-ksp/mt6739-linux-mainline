#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cp .config /mnt/d/y5_linux_work/out/config-before-v51
cp /mnt/d/y5_linux_work/clk-mt6739-boot-cpu.c drivers/clk/mediatek/
cp /mnt/d/y5_linux_work/mfw-cpu-rate.h drivers/clk/mediatek/
cp /mnt/d/y5_linux_work/mfw-charger-state.h drivers/power/supply/
cp /mnt/d/y5_linux_work/y5-pmic-monitor.c drivers/power/supply/
python3 - <<'PY'
from pathlib import Path
p=Path('drivers/clk/mediatek/Kconfig'); s=p.read_text()
if 'config COMMON_CLK_MT6739_BOOT_CPU' not in s:
 s+='''
config COMMON_CLK_MT6739_BOOT_CPU
	bool "MT6739 read-only boot CPU clock"
	depends on COMMON_CLK && MFD_SYSCON
	help
	  Expose the LK CPU clock through CCF without changing PLL or voltage.
'''
 p.write_text(s)
p=Path('drivers/clk/mediatek/Makefile'); s=p.read_text()
if 'clk-mt6739-boot-cpu.o' not in s:
 p.write_text(s+'\nobj-$(CONFIG_COMMON_CLK_MT6739_BOOT_CPU) += clk-mt6739-boot-cpu.o\n')
PY
scripts/config --enable MFD_SYSCON --enable COMMON_CLK_MT6739_BOOT_CPU
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig > /mnt/d/y5_linux_work/out/v51-config.log 2>&1
grep -x 'CONFIG_COMMON_CLK_MT6739_BOOT_CPU=y' .config
grep -E 'CONFIG_(KEXEC|CPU_FREQ|MFD_SYSCON)' .config
cd /mnt/d/y5_linux_work
gcc -O2 -Wall -Wextra -Werror -fsanitize=undefined -o /tmp/test_mfw_v51 test_mfw_v51.c
/tmp/test_mfw_v51
cd /home/y5/linux-7.3
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j2 drivers/power/supply/y5-pmic-monitor.o drivers/clk/mediatek/clk-mt6739-boot-cpu.o > /mnt/d/y5_linux_work/out/v51-compile.log 2>&1
nohup bash /mnt/d/y5_linux_work/bld.sh >/dev/null 2>&1 < /dev/null &
wait "$!"
