#!/bin/bash
cd /home/y5/linux-7.3
export ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
echo "=== START $(date)"
make -j8 Image mediatek/mt6739-huawei-dra.dtb 2>&1 | tail -n 40
echo "=== rc=${PIPESTATUS[0]} $(date)"
ls -l arch/arm64/boot/Image arch/arm64/boot/dts/mediatek/mt6739-huawei-dra.dtb