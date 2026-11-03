#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v5.dtsi .
cat stock_0.dts extras_v5.dtsi > v5.dts
dtc -I dts -O dtb -o v5.dtb v5.dts 2>dtc_v5.err || true
ls -l v5.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v5.err
echo "--- magic: $(head -c4 v5.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v5.dtb)"
