#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v2.dtsi .
cat stock_0.dts extras_v2.dtsi > v2.dts
dtc -I dts -O dtb -o v2.dtb v2.dts 2>dtc_v2.err || true
ls -l v2.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v2.err
echo "--- magic: $(head -c4 v2.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v2.dtb)"
