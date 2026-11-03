#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v3.dtsi .
cat stock_0.dts extras_v3.dtsi > v3.dts
dtc -I dts -O dtb -o v3.dtb v3.dts 2>dtc_v3.err || true
ls -l v3.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v3.err
echo "--- magic: $(head -c4 v3.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v3.dtb)"
