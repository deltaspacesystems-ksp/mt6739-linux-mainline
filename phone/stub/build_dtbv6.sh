#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v6.dtsi .
cat stock_0.dts extras_v6.dtsi > v6.dts
dtc -I dts -O dtb -o v6.dtb v6.dts 2>dtc_v6.err || true
ls -l v6.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v6.err
echo "--- magic: $(head -c4 v6.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v6.dtb)"
