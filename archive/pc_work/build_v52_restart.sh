#!/bin/bash
set -eu
cd /home/y5/linux-7.3
python3 - <<'PY'
from pathlib import Path
p=Path('drivers/watchdog/mtk_wdt.c')
s=p.read_text()
old='\twatchdog_set_restart_priority(&mtk_wdt->wdt_dev, 128);'
new='''	/* DURA-L21: use TOPRGU before the generic PSCI reset path. */
	if (of_machine_is_compatible("huawei,dura-l21")) {
		watchdog_set_restart_priority(&mtk_wdt->wdt_dev, 192);
		dev_info(dev, "DURA-L21: TOPRGU restart priority 192 (before PSCI)\\n");
	} else {
		watchdog_set_restart_priority(&mtk_wdt->wdt_dev, 128);
	}'''
if 'DURA-L21: TOPRGU restart priority 192' not in s:
 assert s.count(old)==1
 backup=Path('/mnt/d/y5_linux_work/out/mtk_wdt-before-v52.c')
 if not backup.exists(): backup.write_text(s)
 p.write_text(s.replace(old,new))
PY
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- olddefconfig > /mnt/d/y5_linux_work/out/v52-config.log 2>&1
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -j2 drivers/watchdog/mtk_wdt.o > /mnt/d/y5_linux_work/out/v52-compile.log 2>&1
nohup bash /mnt/d/y5_linux_work/bld.sh >/dev/null 2>&1 < /dev/null &
wait "$!"
