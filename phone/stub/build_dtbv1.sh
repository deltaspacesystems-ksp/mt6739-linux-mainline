#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v1.dtsi .
cat stock_0.dts extras_v1.dtsi > v1.dts
dtc -I dts -O dtb -o v1.dtb v1.dts 2>dtc_v1.err || true
ls -l v1.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v1.err
echo "--- magic: $(head -c4 v1.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v1.dtb)"
