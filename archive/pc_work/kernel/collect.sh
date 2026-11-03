#!/bin/bash
K=/home/y5/linux-7.3/arch/arm64/boot
ls -l $K/Image $K/Image.gz $K/dts/mediatek/mt6739-huawei-dra.dtb 2>&1
mkdir -p /mnt/d/y5_linux_work/out && cp $K/Image $K/dts/mediatek/mt6739-huawei-dra.dtb /mnt/d/y5_linux_work/out/ && cp /home/y5/linux-7.3/.config /mnt/d/y5_linux_work/out/kernel7.3.config && echo copied
