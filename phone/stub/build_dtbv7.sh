#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v7.dtsi .
cat stock_0.dts extras_v7.dtsi > v7.dts
dtc -I dts -O dtb -o v7.dtb v7.dts 2>dtc_v7.err || true
ls -l v7.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v7.err
echo "--- magic: $(head -c4 v7.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v7.dtb)"
