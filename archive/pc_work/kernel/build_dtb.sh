#!/bin/bash
set -e
K=/home/y5/linux-7.3; S=/mnt/d/y5_linux_work/kernel
cd $K
cp $S/mt6739.dtsi $S/mt6739-huawei-dra.dts arch/arm64/boot/dts/mediatek/
grep -q 'mt6739-huawei-dra' arch/arm64/boot/dts/mediatek/Makefile || echo 'dtb-$(CONFIG_ARCH_MEDIATEK) += mt6739-huawei-dra.dtb' >> arch/arm64/boot/dts/mediatek/Makefile
export ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
make defconfig >/dev/null
cat $S/y5.config >> .config
make olddefconfig >/dev/null
echo "config ok: $(grep -c '=y' .config) opcji =y"
grep -E 'CONFIG_(ARCH_MEDIATEK|DRM_SIMPLEDRM|SYSFB_SIMPLEFB|FRAMEBUFFER_CONSOLE|PSTORE_RAM|SERIAL_8250_MT6577)=' .config
make -j8 mediatek/mt6739-huawei-dra.dtb 2>&1 | tail -n 25
ls -l arch/arm64/boot/dts/mediatek/mt6739-huawei-dra.dtb