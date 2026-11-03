#!/bin/bash
set -e
cd /mnt/d/y5_linux_work/out
cp /mnt/c/Users/pc/Documents/y5_linux_work/extras_v8.dtsi .
cat stock_0.dts extras_v8.dtsi > v8.dts
dtc -I dts -O dtb -o v8.dtb v8.dts 2>dtc_v8.err || true
ls -l v8.dtb stock_0.dtb
echo "--- ostrzezenia dtc (pierwsze 8 linii):"; head -8 dtc_v8.err
echo "--- magic: $(head -c4 v8.dtb | xxd -p 2>/dev/null || od -An -tx1 -N4 v8.dtb)"
