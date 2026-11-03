#!/bin/bash
set -eu
cd /home/y5/linux-7.3
cmp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v58
cmp drivers/power/supply/y5-pmic-monitor.c /mnt/d/y5_linux_work/export/y5-pmic-before-v59.c
cp /mnt/d/y5_linux_work/export/y5-pmic-v59.c drivers/power/supply/y5-pmic-monitor.c
cp /mnt/d/y5_linux_work/microfw-gpu/clk-mfw-mfg-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-clock-math.h drivers/clk/mediatek/
if ! grep -q '^obj-y += clk-mfw-mfg-probe.o$' drivers/clk/mediatek/Makefile; then printf '\nobj-y += clk-mfw-mfg-probe.o\n' >> drivers/clk/mediatek/Makefile; fi
cp /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id-probe.c /mnt/d/y5_linux_work/microfw-gpu/mfw-gpu-id.h /mnt/d/y5_linux_work/microfw-gpu/mfw-mfg-power-sequence.h drivers/gpu/drm/mfw-gpu-probe/
if ! grep -q '^obj-y += mfw-gpu-probe/$' drivers/gpu/drm/Makefile; then printf '\nobj-y += mfw-gpu-probe/\n' >> drivers/gpu/drm/Makefile; fi
touch /mnt/d/y5_linux_work/out/v59-build-start
nohup bash /mnt/d/y5_linux_work/bld.sh > /dev/null 2>&1 &
build_pid=$!
wait "$build_pid"
test arch/arm64/boot/Image -nt /mnt/d/y5_linux_work/out/v59-build-start
cp arch/arm64/boot/Image /mnt/d/y5_linux_work/out/Image_mfw_v59
cp .config /mnt/d/y5_linux_work/export/config-7.3-mfw-v59
cat include/generated/utsversion.h
tail -n 5 /home/y5/build73f.log
