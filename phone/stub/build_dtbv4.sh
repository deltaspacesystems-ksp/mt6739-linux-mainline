#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v4.dtsi .
cat stock_0.dts extras_v4.dtsi > v4.dts
dtc -I dts -O dtb -o v4.dtb v4.dts 2>dtc_v4.err || true
ls -l v4.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v4.err
echo "--- magic: $(head -c4 v4.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v4.dtb)"
