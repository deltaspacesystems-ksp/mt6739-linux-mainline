#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v9.dtsi .
cat stock_0.dts extras_v9.dtsi > v9.dts
dtc -I dts -O dtb -o v9.dtb v9.dts 2>dtc_v9.err || true
ls -l v9.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v9.err
echo "--- magic: $(head -c4 v9.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v9.dtb)"
